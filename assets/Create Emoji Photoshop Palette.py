def color_palette_256():
    colors = []

    # Standard colors (0-15)
    standard_colors = [
        (255, 255, 255), (255, 0, 0), (255, 127, 0), (255, 255, 0), (0, 255, 0),
        (0, 0, 255), (255, 0, 255), (192, 127, 0), (0, 0, 0)
    ]
    colors.extend(standard_colors)
    
    for i in range(247):
        colors.append((0, 0, 0))

    return colors

def dump_to_hex(colors):
    with open('Emoji.act', 'wb') as f:
        for color in colors:
            # Pack each color into bytes and write to the file as raw bytes
            f.write(bytes(color))

if __name__ == "__main__":
    colors = color_palette_256()
    dump_to_hex(colors)
