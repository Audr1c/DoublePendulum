import matplotlib.pyplot as plt
import numpy as np
import json

jsonFile = "colorsmaps.json"
numberOfancherPoint = 60
data = {}
# get the name of all colormaps
colormaps = plt.colormaps()
for cmap in colormaps:
    name = str(cmap)
    print("Doing cmap", name)
    data[name] = {"name": name, "size": numberOfancherPoint}
    anchors = np.linspace(0, 1, numberOfancherPoint)  # pick 10 anchors, or more
    cmap = plt.get_cmap(cmap)
    colors = cmap(anchors)[:, :3]    # drop alpha, keep RGB
    
    
    # Sample anchor points (matplotlib stores them internally at 256, but you can extract fine-grained)
    string = ""
    for i, (x, rgb) in enumerate(zip(anchors, colors)):
        string+=f"{{{x:.6f}, {rgb[0]:.6f}, {rgb[1]:.6f}, {rgb[2]:.6f}}},"
    data[name]["string"] = string
    
# Save to json
with open(jsonFile, "w") as f:
    json.dump(data, f, indent=4)
