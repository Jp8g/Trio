import os

# Base directory is where this script lives
script_dir = os.path.dirname(os.path.abspath(__file__))

module_dir = os.path.join(script_dir, "modules")
core_include_dir = os.path.join(script_dir, "core", "include")
output_file = os.path.join(script_dir, "trio.h")

with open(output_file, "w") as out:
    out.write("// Auto-generated trio umbrella header\n\n")

    # Modules
    for module_name in sorted(os.listdir(module_dir)):
        module_path = os.path.join(module_dir, module_name)
        if not os.path.isdir(module_path):
            continue

        for root, dirs, files in os.walk(module_path):
            for file in sorted(files):
                if file.endswith(".h"):
                    # path relative to script_dir
                    rel_path = os.path.relpath(os.path.join(root, file), script_dir)
                    out.write(f'#include "{rel_path}"\n')

    # Core headers
    for file in sorted(os.listdir(core_include_dir)):
        if file.endswith(".h"):
            rel_path = os.path.relpath(os.path.join(core_include_dir, file), script_dir)
            out.write(f'#include "{rel_path}"\n')
