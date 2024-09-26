def color_palette_256():
    colors = []

    # Standard colors (0-15)
    standard_colors = [
        (0, 0, 0), (128, 0, 0), (0, 128, 0), (128, 128, 0),
        (0, 0, 128), (128, 0, 128), (0, 128, 128), (192, 192, 192),
        (128, 128, 128), (255, 0, 0), (0, 255, 0), (255, 255, 0),
        (0, 0, 255), (255, 0, 255), (0, 255, 255), (255, 255, 255)
    ]
    colors.extend(standard_colors)

    # 6x6x6 color cube (16-231)
    for r in range(6):
        for g in range(6):
            for b in range(6):
                colors.append((r * 51, g * 51, b * 51))  # Values: 0, 51, 102, 153, 204, 255

    # Grayscale ramp (232-255)
    for i in range(24):
        shade = 8 + i * 10  # From 8 to 248
        colors.append((shade, shade, shade))

    return colors

def dump_to_hex(colors):
    with open('ANSI.act', 'wb') as f:
        for color in colors:
            # Pack each color into bytes and write to the file as raw bytes
            f.write(bytes(color))

if __name__ == "__main__":
    colors = color_palette_256()
    dump_to_hex(colors)
