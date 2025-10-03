#!/bin/bash

# Script de test complet pour la validation des maps cub3D
# Usage: ./test_maps.sh

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0
# Valgrind configuration
# - run-libc-freeres reduces noisy 'still reachable' from glibc caches
# - show-leak-kinds=all keeps visibility, but we won't fail on 'still reachable' by default
# You can export STRICT_STILL_REACH=1 to also fail on 'still reachable'.
VALGRIND_FLAGS="--leak-check=full --show-leak-kinds=all --track-origins=yes --run-libc-freeres=yes --suppressions=mlx.supp"

# Strictness toggle (set STRICT_STILL_REACH=0 to tolerate 'still reachable')
# Default is STRICT: count 'still reachable' as failures as requested
STRICT_STILL_REACH=${STRICT_STILL_REACH:-1}

# Valgrind log options
# KEEP_VGLOG=1 to keep per-test logs (default 0 deletes them)
# VERBOSE_VG=1 to print the full valgrind log for each test (noisy)
# VGLOG_DIR to choose where logs are stored (default /tmp)
KEEP_VGLOG=${KEEP_VGLOG:-0}
VERBOSE_VG=${VERBOSE_VG:-0}
VGLOG_DIR=${VGLOG_DIR:-/tmp}

# Ensure log directory exists
mkdir -p "$VGLOG_DIR" 2>/dev/null || true

echo "========================================="
echo "  cub3D Comprehensive Validation Tests"
echo "========================================="
echo ""

# Vérifier que le programme est compilé
if [ ! -f "./cub3D" ]; then
    echo -e "${RED}Error: ./cub3D not found. Run 'make' first.${NC}"
    exit 1
fi

# Fonction de test pour maps valides
test_valid_map() {
    local map_file=$1
    local description=$2

    echo ""
    echo -e "${BLUE}Testing VALID:${NC} $description"
    echo "File: $map_file"

    # Exécuter en mode parse-only (pas de fenêtre/loop) pour une sortie propre
    local map_base=$(basename "$map_file")
    local map_stem="${map_base%.*}"
    local status="valid"
    local ts=$(date +%Y%m%d_%H%M%S)
    VGLOG="$VGLOG_DIR/${map_stem}_${status}_${ts}.log"
    valgrind $VALGRIND_FLAGS --log-file="$VGLOG" ./cub3D --parse-only "$map_file" > /dev/null 2>&1
    local exit_code=$?

    # Extraire les fuites mémoire — supprimer les séparateurs de milliers éventuels
    local def_lost=$(grep "definitely lost:" "$VGLOG" | awk '{gsub(/,/, "", $4); print $4}')
    local ind_lost=$(grep "indirectly lost:" "$VGLOG" | awk '{gsub(/,/, "", $4); print $4}')
    local still_reach=$(grep "still reachable:" "$VGLOG" | awk '{gsub(/,/, "", $4); print $4}')

    # Calculer le total des fuites (ignorer les valeurs vides)
    # Par défaut (strict), on COMPTE 'still reachable' comme fuite bloquante
    local sr=0
    if [ "$STRICT_STILL_REACH" = "1" ]; then sr=${still_reach:-0}; fi
    local total_leaks=$(( ${def_lost:-0} + ${ind_lost:-0} + ${sr:-0} ))

    # 0 = exit propre
    if [ $exit_code -eq 0 ]; then
        if [ $total_leaks -eq 0 ]; then
            [ "$STRICT_STILL_REACH" = "1" ] \
                && echo -e "${GREEN}✓ PASS${NC} - Map acceptée, pas de fuites (incl. still reachable) (exit: $exit_code)" \
                || echo -e "${GREEN}✓ PASS${NC} - Map acceptée, pas de fuites bloquantes (exit: $exit_code). still_reachable=${still_reach:-0} bytes (toléré)"
            ((PASSED++))
        else
            echo -e "${RED}✗ FAIL${NC} - Memory leaks: def=${def_lost:-0} ind=${ind_lost:-0} still=${still_reach:-0} (total compté: $total_leaks bytes)"
            ((FAILED++))
        fi
    else
        echo -e "${RED}✗ FAIL${NC} - Map rejetée alors qu'elle devrait être valide (exit: $exit_code)"
        grep "Error" "$VGLOG" 2>/dev/null || echo "No error message found"
        ((FAILED++))
    fi

    if [ "$VERBOSE_VG" = "1" ]; then
        echo -e "\n---- Valgrind log ($VGLOG) ----"
        cat "$VGLOG"
        echo -e "---- End Valgrind log ----\n"
    fi

    if [ "$KEEP_VGLOG" = "1" ]; then
        echo "Valgrind log saved: $VGLOG"
    else
        rm -f "$VGLOG"
    fi
}

# Fonction de test pour maps invalides
test_invalid_map() {
    local map_file=$1
    local description=$2

    echo ""
    echo -e "${YELLOW}Testing INVALID:${NC} $description"
    echo "File: $map_file"

    # Exécuter en mode parse-only (pas de fenêtre/loop) pour une sortie propre
    local map_base=$(basename "$map_file")
    local map_stem="${map_base%.*}"
    local status="invalid"
    local ts=$(date +%Y%m%d_%H%M%S)
    VGLOG="$VGLOG_DIR/valgrind_${map_stem}_${status}_${ts}.log"
    valgrind $VALGRIND_FLAGS --log-file="$VGLOG" ./cub3D --parse-only "$map_file" > /dev/null 2>&1
    local exit_code=$?

    # Extraire les fuites mémoire — supprimer les séparateurs de milliers éventuels
    local def_lost=$(grep "definitely lost:" "$VGLOG" | awk '{gsub(/,/, "", $4); print $4}')
    local ind_lost=$(grep "indirectly lost:" "$VGLOG" | awk '{gsub(/,/, "", $4); print $4}')
    local still_reach=$(grep "still reachable:" "$VGLOG" | awk '{gsub(/,/, "", $4); print $4}')

    # Calculer le total des fuites. Par défaut (strict), 'still reachable' est bloquant
    local sr=0
    if [ "$STRICT_STILL_REACH" = "1" ]; then sr=${still_reach:-0}; fi
    local total_leaks=$(( ${def_lost:-0} + ${ind_lost:-0} + ${sr:-0} ))

    # La map doit être rejetée (exit code != 0)
    if [ $exit_code -ne 0 ]; then
        if [ $total_leaks -eq 0 ]; then
            [ "$STRICT_STILL_REACH" = "1" ] \
                && echo -e "${GREEN}✓ PASS${NC} - Map rejetée correctement, pas de fuites (incl. still reachable)" \
                || echo -e "${GREEN}✓ PASS${NC} - Map rejetée correctement, pas de fuites bloquantes. still_reachable=${still_reach:-0} bytes (toléré)"
            ((PASSED++))
        else
            echo -e "${RED}✗ FAIL${NC} - Memory leaks: def=${def_lost:-0} ind=${ind_lost:-0} still=${still_reach:-0} (total compté: $total_leaks bytes)"
            ((FAILED++))
        fi
    else
        echo -e "${RED}✗ FAIL${NC} - Map acceptée alors qu'elle devrait être invalide (exit: $exit_code)"
        ((FAILED++))
    fi

    if [ "$VERBOSE_VG" = "1" ]; then
        echo -e "\n---- Valgrind log ($VGLOG) ----"
        cat "$VGLOG"
        echo -e "---- End Valgrind log ----\n"
    fi

    if [ "$KEEP_VGLOG" = "1" ]; then
        echo "Valgrind log saved: $VGLOG"
    else
        rm -f "$VGLOG"
    fi
}

# Fonction de test spécial pour les cas sans fichier .cub
test_special_case() {
    local cmd=$1
    local description=$2

    echo ""
    echo -e "${YELLOW}Testing SPECIAL:${NC} $description"
    echo "Command: $cmd"

    eval "timeout 2s $cmd > /dev/null 2>&1"
    local exit_code=$?

    if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
        echo -e "${GREEN}✓ PASS${NC} - Erreur détectée correctement (exit: $exit_code)"
        ((PASSED++))
    else
        echo -e "${RED}✗ FAIL${NC} - Devrait rejeter (exit: $exit_code)"
        ((FAILED++))
    fi
}

# Compter les maps disponibles
VALID_COUNT=$(find maps/valid -name "*.cub" 2>/dev/null | wc -l | tr -d ' ')
ROOT_VALID_COUNT=$(find maps -maxdepth 1 -name "*.cub" 2>/dev/null | wc -l | tr -d ' ')
TOTAL_VALID=$((VALID_COUNT + ROOT_VALID_COUNT))

echo ""
echo "========================================="
echo "   VALID MAPS TESTS ($TOTAL_VALID maps)"
echo "========================================="

# Test des maps valides dans valid/
if [ -d "maps/valid" ]; then
    for map in maps/valid/*.cub; do
        if [ -f "$map" ]; then
            basename=$(basename "$map" .cub)
            test_valid_map "$map" "$basename"
        fi
    done
fi

# Test des maps valides à la racine de maps/
for map in maps/*.cub; do
    if [ -f "$map" ]; then
        basename=$(basename "$map" .cub)
        test_valid_map "$map" "$basename (root)"
    fi
done

# Compter les maps invalides disponibles
INVALID_COUNT=$(find maps/invalid -name "*.cub" 2>/dev/null | wc -l | tr -d ' ')

echo ""
echo "========================================="
echo "  INVALID MAPS TESTS ($((INVALID_COUNT + 2)) tests)"
echo "========================================="

# Tests spéciaux (arguments)
test_special_case "./cub3D" "No arguments"
test_special_case "./cub3D maps/nonexistent.txt" "Wrong file extension (.txt)"

# Test de toutes les maps invalides dans invalid/
if [ -d "maps/invalid" ]; then
    for map in maps/invalid/*.cub; do
        if [ -f "$map" ]; then
            basename=$(basename "$map" .cub)
            test_invalid_map "$map" "$basename"
        fi
    done
fi

echo ""
echo "========================================="
echo "             SUMMARY"
echo "========================================="
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"
echo "Total:  $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 All tests passed! Your parser is solid! 🎉${NC}"
    exit 0
else
    echo -e "${RED}⚠️  Some tests failed. Check the output above. ⚠️${NC}"
    exit 1
fi
