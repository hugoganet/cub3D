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
VALGRIND_FLAGS="--leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=mlx.supp --log-file=/tmp/valgrind_test.log"

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

    # Exécuter avec valgrind (timeout 2s)
    timeout 2s valgrind $VALGRIND_FLAGS ./cub3D "$map_file" > /dev/null 2>&1
    local exit_code=$?

    # Vérifier TOUTES les fuites mémoire (definitely lost, indirectly lost, still reachable)
    local def_lost=$(grep "definitely lost:" /tmp/valgrind_test.log | awk '{print $4}')
    local ind_lost=$(grep "indirectly lost:" /tmp/valgrind_test.log | awk '{print $4}')
    local still_reach=$(grep "still reachable:" /tmp/valgrind_test.log | awk '{print $4}')

    # Calculer le total des fuites (ignorer les valeurs vides)
    local total_leaks=$(( ${def_lost:-0} + ${ind_lost:-0} + ${still_reach:-0} ))

    # 124 = timeout (normal pour maps valides qui lancent la fenêtre)
    # 0 = exit propre (si X11 pas disponible mais parsing OK)
    if [ $exit_code -eq 0 ] || [ $exit_code -eq 124 ]; then
        if [ $total_leaks -eq 0 ]; then
            echo -e "${GREEN}✓ PASS${NC} - Map acceptée, pas de fuites (exit: $exit_code)"
            ((PASSED++))
        else
            echo -e "${RED}✗ FAIL${NC} - Memory leaks: def=$def_lost ind=$ind_lost still=$still_reach (total: $total_leaks bytes)"
            ((FAILED++))
        fi
    else
        echo -e "${RED}✗ FAIL${NC} - Map rejetée alors qu'elle devrait être valide (exit: $exit_code)"
        grep "Error" /tmp/valgrind_test.log 2>/dev/null || echo "No error message found"
        ((FAILED++))
    fi

    rm -f /tmp/valgrind_test.log
}

# Fonction de test pour maps invalides
test_invalid_map() {
    local map_file=$1
    local description=$2

    echo ""
    echo -e "${YELLOW}Testing INVALID:${NC} $description"
    echo "File: $map_file"

    # Exécuter avec valgrind
    timeout 2s valgrind $VALGRIND_FLAGS ./cub3D "$map_file" > /dev/null 2>&1
    local exit_code=$?

    # Vérifier TOUTES les fuites mémoire (definitely lost, indirectly lost, still reachable)
    local def_lost=$(grep "definitely lost:" /tmp/valgrind_test.log | awk '{print $4}')
    local ind_lost=$(grep "indirectly lost:" /tmp/valgrind_test.log | awk '{print $4}')
    local still_reach=$(grep "still reachable:" /tmp/valgrind_test.log | awk '{print $4}')

    # Calculer le total des fuites (ignorer les valeurs vides)
    local total_leaks=$(( ${def_lost:-0} + ${ind_lost:-0} + ${still_reach:-0} ))

    # La map doit être rejetée (exit code != 0 et != 124)
    if [ $exit_code -ne 0 ] && [ $exit_code -ne 124 ]; then
        if [ $total_leaks -eq 0 ]; then
            echo -e "${GREEN}✓ PASS${NC} - Map rejetée correctement, pas de fuites"
            ((PASSED++))
        else
            echo -e "${RED}✗ FAIL${NC} - Memory leaks: def=$def_lost ind=$ind_lost still=$still_reach (total: $total_leaks bytes)"
            ((FAILED++))
        fi
    else
        echo -e "${RED}✗ FAIL${NC} - Map acceptée alors qu'elle devrait être invalide (exit: $exit_code)"
        ((FAILED++))
    fi

    rm -f /tmp/valgrind_test.log
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

echo ""
echo "========================================="
echo "       VALID MAPS TESTS (16 tests)"
echo "========================================="

test_valid_map "maps/valid/simple_square.cub" "Simple square map"
test_valid_map "maps/valid/minimal_3x3.cub" "Minimal 3x3 map"
test_valid_map "maps/valid/player_north.cub" "Player facing North"
test_valid_map "maps/valid/player_south.cub" "Player facing South"
test_valid_map "maps/valid/player_east.cub" "Player facing East"
test_valid_map "maps/valid/player_west.cub" "Player facing West"
test_valid_map "maps/valid/irregular_shape.cub" "Irregular map shape"
test_valid_map "maps/valid/spaces_outside_walls.cub" "Spaces outside walls"
test_valid_map "maps/valid/spaces_isolated_sections.cub" "Isolated space sections"
test_valid_map "maps/valid/large_open_area.cub" "Large open area"
test_valid_map "maps/valid/maze_complex.cub" "Complex maze"
test_valid_map "maps/valid/rgb_min_values.cub" "RGB min values (0,0,0)"
test_valid_map "maps/valid/rgb_max_values.cub" "RGB max values (255,255,255)"
test_valid_map "maps/valid/elements_unordered.cub" "Unordered config elements"
test_valid_map "maps/valid/extra_spaces_in_config.cub" "Extra spaces in config"
test_valid_map "maps/valid/empty_lines_before_map.cub" "Empty lines before map"

# Maps originales
if [ -f "maps/sample.cub" ]; then
    test_valid_map "maps/sample.cub" "Original sample.cub"
fi
if [ -f "maps/complex.cub" ]; then
    test_valid_map "maps/complex.cub" "Original complex.cub"
fi

echo ""
echo "========================================="
echo "      INVALID MAPS TESTS (34 tests)"
echo "========================================="

# Tests spéciaux (arguments)
test_special_case "./cub3D" "No arguments"
test_special_case "./cub3D maps/sample.txt" "Wrong file extension (.txt)"

# Tests de configuration manquante
test_invalid_map "maps/invalid/missing_north_texture.cub" "Missing NO texture"
test_invalid_map "maps/invalid/missing_south_texture.cub" "Missing SO texture"
test_invalid_map "maps/invalid/missing_west_texture.cub" "Missing WE texture"
test_invalid_map "maps/invalid/missing_east_texture.cub" "Missing EA texture"
test_invalid_map "maps/invalid/missing_floor_color.cub" "Missing F color"
test_invalid_map "maps/invalid/missing_ceiling_color.cub" "Missing C color"

# Tests RGB invalides
test_invalid_map "maps/invalid/rgb_negative.cub" "RGB negative value"
test_invalid_map "maps/invalid/rgb_over_255.cub" "RGB value > 255"
test_invalid_map "maps/invalid/rgb_non_numeric.cub" "RGB non-numeric value"
test_invalid_map "maps/invalid/rgb_incomplete.cub" "RGB incomplete (only 2 values)"

# Tests joueur
test_invalid_map "maps/invalid/no_player.cub" "No player in map"
test_invalid_map "maps/invalid/multiple_players.cub" "Multiple players in map"

# Tests caractères invalides
test_invalid_map "maps/invalid/invalid_char_X.cub" "Invalid character 'X' in map"
test_invalid_map "maps/invalid/invalid_char_digit.cub" "Invalid character '2' in map"
test_invalid_map "maps/invalid/tab_in_map.cub" "Tab character in map"

# Tests map vide/incomplète
test_invalid_map "maps/invalid/empty_map.cub" "Empty map"

# Tests map non fermée (bords)
test_invalid_map "maps/invalid/map_not_closed_top.cub" "Map not closed (top)"
test_invalid_map "maps/invalid/map_not_closed_bottom.cub" "Map not closed (bottom)"
test_invalid_map "maps/invalid/map_not_closed_left.cub" "Map not closed (left)"
test_invalid_map "maps/invalid/map_not_closed_right.cub" "Map not closed (right)"

# Tests joueur sur les bords
test_invalid_map "maps/invalid/player_on_edge_top.cub" "Player on top edge"
test_invalid_map "maps/invalid/player_on_edge_bottom.cub" "Player on bottom edge"
test_invalid_map "maps/invalid/player_on_edge_left.cub" "Player on left edge"
test_invalid_map "maps/invalid/player_on_edge_right.cub" "Player on right edge"

# Tests espaces dans zone jouable
test_invalid_map "maps/invalid/space_in_playable_area.cub" "Space in playable area"
test_invalid_map "maps/invalid/space_next_to_player.cub" "Space next to player"
test_invalid_map "maps/invalid/hole_in_wall.cub" "Hole in wall (space)"

# Tests floor sur bords
test_invalid_map "maps/invalid/floor_on_last_line.cub" "Floor (0) on edge line"
test_invalid_map "maps/invalid/irregular_open_corner.cub" "Open corner (irregular)"

# Tests newline dans map
test_invalid_map "maps/invalid/newline_in_map.cub" "Newline inside map"

# Tests uniques des maps originales
test_invalid_map "maps/invalid/rgb_malformed_ceiling.cub" "RGB malformed (extra digits)"
test_invalid_map "maps/invalid/duplicate_texture_identifier.cub" "Duplicate texture identifier (NO twice)"

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
