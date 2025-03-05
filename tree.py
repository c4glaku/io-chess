import os
import sys
import stat
import fnmatch
from pathlib import Path

# Usage: python -X utf8 tree.py > output_file.txt


# Ensure UTF-8 encoding (if using Python 3.7+)
sys.stdout.reconfigure(encoding='utf-8')

# List of ignore patterns (from your .gitignore)
ignore_patterns = [
    "node_modules",    # Node Modules
    ".env",            # Environment file
    "logs",            # Logs directory
    "*.log",           # Any file ending with .log
    "npm-debug.log*",  # NPM debug logs
    "pids",            # PIDs directory
    "*.pid",           # PID files
    "*.seed",          # Seed files
    "dist",            # Build directory
    # "build",           # Build directory
    ".vscode",         # IDE-specific directory
    ".idea",           # IDE-specific directory
    "*.swp",           # Swap files
    ".DS_Store"        # macOS metadata
    "tree.py"          # This script
    "project_structure.txt"  # Output file
]

def is_hidden(file_path):
    """Return True if the file/directory is hidden."""
    if file_path.name.startswith("."):
        return True  # Unix-based hidden file/folder
    try:
        return bool(file_path.stat().st_file_attributes & stat.FILE_ATTRIBUTE_HIDDEN)
    except AttributeError:
        return False  # Non-Windows systems

def should_ignore(item):
    """Return True if the item matches any ignore pattern."""
    return any(fnmatch.fnmatch(item.name, pattern) for pattern in ignore_patterns)

def print_directory_tree(startpath):
    startpath = Path(startpath).resolve()
    root_name = startpath.name
    print(f"{root_name}/")

    prefix = {
        "dir": "├── ",
        "last_dir": "└── ",
        "indent": "│   ",
        "last_indent": "    "
    }
    
    visited = set()  # To avoid infinite loops

    def generate_tree(path, level=0, is_last_item=[True]):
        if path in visited:
            return
        visited.add(path)
        
        try:
            contents = [
                item for item in path.iterdir()
                if not is_hidden(item) and not should_ignore(item) and not item.is_symlink()
            ]
        except PermissionError:
            return  # Skip folders that can't be accessed

        contents.sort(key=lambda x: (not x.is_dir(), x.name))
        
        for index, item in enumerate(contents):
            is_last = (index == len(contents) - 1)
            line = ""
            for i in range(level):
                line += prefix["last_indent"] if is_last_item[i] else prefix["indent"]
            line += prefix["last_dir"] if is_last else prefix["dir"]
            
            print(f"{line}{item.name}")
            
            if item.is_dir():
                is_last_item.append(is_last)
                generate_tree(item, level + 1, is_last_item)
                is_last_item.pop()

    generate_tree(startpath)

if __name__ == "__main__":
    print_directory_tree(".")
