with open("t.js", "r") as obj:
    vs = []
    fs = []
    
    for line in obj:
        parts = line.strip().split()
        
        if len(parts) == 0:
            continue
            
        if parts[0] == 'v':
            vs.append({
                'x': float(parts[1]),
                'y': float(parts[2]),
                'z': float(parts[3])
            })
        
        elif parts[0] == 'f':
            # Extract vertex indices (handle format like "1/2/3")
            face = [int(p.split('/')[0]) - 1 for p in parts[1:]]
            fs.append(face)

# Find min/max for normalization
min_x = min(v['x'] for v in vs)
max_x = max(v['x'] for v in vs)
min_y = min(v['y'] for v in vs)
max_y = max(v['y'] for v in vs)
min_z = min(v['z'] for v in vs)
max_z = max(v['z'] for v in vs)

# Normalize to -1..1 range
for v in vs:
    v['x'] = 2 * (v['x'] - min_x) / (max_x - min_x) - 1
    v['y'] = 2 * (v['y'] - min_y) / (max_y - min_y) - 1
    v['z'] = 2 * (v['z'] - min_z) / (max_z - min_z) - 1
    
    # Push z further away so it's visible (adjust this value as needed)
    v['z'] += 3  # Add offset to move model away from camera

# Save to a new JavaScript file
with open("model_data.js", "w") as output:
    output.write("const vs = [\n")
    for i, v in enumerate(vs):
        comma = "," if i < len(vs) - 1 else ""
        output.write(f"    {{x: {v['x']}, y: {v['y']}, z: {v['z']}}}{comma}\n")
    output.write("];\n\n")
    
    output.write("const fs = [\n")
    for i, f in enumerate(fs):
        comma = "," if i < len(fs) - 1 else ""
        output.write(f"    {f}{comma}\n")
    output.write("];\n")

print(f"Converted {len(vs)} vertices and {len(fs)} faces to model_data.js")
print(f"Normalized from x:[{min_x:.2f}, {max_x:.2f}] y:[{min_y:.2f}, {max_y:.2f}] z:[{min_z:.2f}, {max_z:.2f}] to -1..1")
print(f"Z offset applied: +3")