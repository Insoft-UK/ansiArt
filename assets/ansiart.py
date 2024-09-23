import random

# Define ANSI color codes
colors = {
    "red": "\033[31m",
    "green": "\033[32m",
    "yellow": "\033[33m",
    "blue": "\033[34m",
    "magenta": "\033[35m",
    "cyan": "\033[36m",
    "white": "\033[37m",
    "reset": "\033[0m"
}

# Sample character patterns for the art
patterns = ["@", "#", "&", "%", "*", "+", "~", "."]

def generate_ansi_art(width, height):
    art = ""
    for _ in range(height):
        for _ in range(width):
            # Randomly pick a color and a pattern
            color = random.choice(list(colors.values())[:-1])
            pattern = random.choice(patterns)
            art += f"{color}{pattern}{colors['reset']}"
        art += "\n"
    return art

# Generate and display ANSI Art
print(generate_ansi_art(40, 20))
