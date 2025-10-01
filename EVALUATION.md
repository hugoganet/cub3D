# cub3D Evaluation Sheet

## Project Information
- **Project**: cub3D
- **Language**: C
- **Norm**: Norminette
- **Allowed functions**: open, close, read, write, printf, malloc, free, perror, strerror, exit, math library (-lm), MiniLibX functions

## Preliminary Tests

### Defense
- The student(s) must be present for the entire duration of the defense
- Verify that the Git repository belongs to the student(s)
- Check that the project compiles without errors
- Check that no forbidden functions are used
- Ensure the project follows the Norm

### Tests
- Clone the repository
- Compile the project with `make`
- Prepare several `.cub` files for testing (valid and invalid)

## Mandatory Part

### Grading Scale
- Outstanding: 5 points
- Excellent: 4 points
- Good: 3 points
- Satisfactory: 2 points
- Failing: 0 points

### Errors Management (Error handling)

**Rate from 0 (failed) to 5 (excellent)**

- Check that the program displays "Error\n" followed by an explicit error message when:
  - Wrong number of arguments
  - Invalid file extension (must be `.cub`)
  - Invalid configuration file
  - Invalid map (not closed, invalid characters, etc.)
  - Invalid textures (missing files, wrong format, etc.)
  - Invalid RGB colors (out of range, wrong format, etc.)

**Questions to verify:**
- Does the program handle errors correctly?
- Are error messages clear and explicit?
- Does the program exit properly after displaying an error?

### Wall Rendering (Raycasting)

**Rate from 0 (failed) to 5 (excellent)**

- Verify that the program displays a 3D view from a first-person perspective
- Check that walls are rendered correctly
- Verify that different textures are applied to walls based on their orientation (North, South, East, West)
- Check that there is no "fisheye" effect (walls should appear straight)

**Questions to verify:**
- Are the walls rendered in 3D?
- Are the textures correctly applied?
- Is the perspective correct?

### Colors (Floor and Ceiling)

**Rate from 0 (failed) to 5 (excellent)**

- Verify that the floor has the correct color as specified in the `.cub` file
- Verify that the ceiling has the correct color as specified in the `.cub` file
- Check that colors are uniform and rendered correctly

**Questions to verify:**
- Are floor and ceiling colors correctly set?
- Do the colors match the configuration file?

### Minimap (Bonus - if implemented)

**Rate from 0 (failed) to 5 (excellent)**

This section only applies if the bonus part is implemented.

- Check if a minimap is displayed
- Verify that the minimap shows the player's position
- Verify that the minimap shows walls and empty spaces
- Check if the minimap updates in real-time

### Movement and Rotation

**Rate from 0 (failed) to 5 (excellent)**

- Test the following controls:
  - **W/A/S/D** or **Arrow keys**: Move forward, left, backward, right
  - **Left/Right arrows**: Rotate the view left and right
  - **ESC**: Close the window and exit cleanly
  - **Red cross** on window: Close the window and exit cleanly

**Questions to verify:**
- Can the player move in all four directions?
- Does the view rotate smoothly?
- Are collisions with walls detected?
- Does the program exit cleanly when pressing ESC or clicking the red cross?

### Parsing

**Rate from 0 (failed) to 5 (excellent)**

Test the parser with various `.cub` files:

**Valid configurations:**
- Correct texture paths
- Valid RGB values (0-255)
- Valid map (closed by walls, single player spawn)
- Player can spawn facing N, S, E, or W

**Invalid configurations to test:**
- Missing texture
- Invalid RGB values (negative, > 255, non-numeric)
- Map not closed by walls
- Multiple player spawns
- No player spawn
- Invalid characters in map
- Empty lines in the map

**Questions to verify:**
- Does the parser correctly validate all elements?
- Are error messages explicit for each type of error?

## Bonus Part (Optional)

**Only evaluate bonuses if the mandatory part is PERFECT**

### Wall Collisions

**Rate from 0 (failed) to 5 (excellent)**

- Check if wall collisions are smooth and realistic
- Verify that the player cannot pass through walls
- Test collision detection from different angles

### Minimap

**Rate from 0 (failed) to 5 (excellent)**

- Verify minimap is displayed
- Check if it shows player position and orientation
- Verify walls and spaces are correctly represented
- Check real-time updates

### Doors

**Rate from 0 (failed) to 5 (excellent)**

- Check if doors can be opened and closed
- Verify door animations (if any)
- Test door interaction (key to open/close)

### Animated Sprites

**Rate from 0 (failed) to 5 (excellent)**

- Check if sprites are displayed
- Verify sprite animations
- Test sprite positioning and scaling

### Mouse Rotation

**Rate from 0 (failed) to 5 (excellent)**

- Check if mouse movement rotates the view
- Verify rotation is smooth and responsive
- Test mouse sensitivity

## Memory Leaks

**Critical Check - Can invalidate the project**

- Run the program with Valgrind or similar tool:
  ```bash
  valgrind --leak-check=full ./cub3D maps/valid_map.cub
  ```
- Check for memory leaks when:
  - Exiting normally (ESC key)
  - Closing window (red cross)
  - Encountering errors
  - Running the game loop

**All allocated memory must be properly freed. Any leak fails the project.**

## Compilation

- `make` must compile without warnings
- `make clean` must remove object files
- `make fclean` must remove object files and executable
- `make re` must recompile everything
- No relink should occur when running `make` twice

## Forbidden Functions

Verify that ONLY the following functions are used:
- open, close, read, write
- printf (or equivalent from ft_printf)
- malloc, free
- perror, strerror, exit
- All math library functions (`-lm`)
- All MiniLibX functions

**Any other function is forbidden and fails the project.**

## General Evaluation

### Code Quality
- Is the code well-organized and readable?
- Are functions short and focused (Norm compliance)?
- Is the project structure logical?

### Understanding
- Can the student explain:
  - How raycasting works?
  - How textures are mapped to walls?
  - How the DDA algorithm works?
  - How player movement and rotation are implemented?

## Final Grade Calculation

- **Mandatory Part**: Maximum 100 points
  - Errors Management: 20 points max
  - Wall Rendering: 25 points max
  - Colors: 15 points max
  - Movement and Rotation: 25 points max
  - Parsing: 15 points max

- **Bonus Part**: Maximum 25 points (only if mandatory is perfect)
  - Each bonus feature: 5 points max

**Total Maximum: 125 points**

## Important Notes

1. **Memory leaks = automatic fail**
2. **Norm errors = automatic fail**
3. **Forbidden functions = automatic fail**
4. **Crash = automatic fail**
5. **Bonuses only count if mandatory part is perfect (100/100)**

## Evaluation Tips

- Test edge cases thoroughly
- Try to break the program with invalid inputs
- Check memory management carefully
- Verify all error messages are explicit
- Test all movement and rotation combinations
- Ensure smooth gameplay and correct rendering
