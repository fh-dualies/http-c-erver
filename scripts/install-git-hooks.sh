#!/bin/sh
#
# Shell script to install pre-commit hook from a specific directory into .git/hooks directory

# Directory where the pre-commit hook is located
HOOKS_DIR="git-hooks"

# Name of the pre-commit hook file
HOOK_FILE="pre-commit"

# Path to the pre-commit hook in the source directory
SRC_HOOK_PATH="$HOOKS_DIR/$HOOK_FILE"

# Path to the pre-commit hook in the .git/hooks directory
DEST_HOOK_PATH=".git/hooks/$HOOK_FILE"

# Check if the pre-commit hook file exists in the source directory
if [ ! -f "$SRC_HOOK_PATH" ]; then
    echo "Error: Pre-commit hook file not found in $HOOKS_DIR"
    exit 1
fi

# Check if the pre-commit hook already exists in the destination directory
if [ -f "$DEST_HOOK_PATH" ]; then
    echo "Error: Pre-commit hook already exists. Please remove or rename it before installation."
    exit 1
fi

# Copy the pre-commit hook from source directory to destination directory
cp "$SRC_HOOK_PATH" "$DEST_HOOK_PATH"
if [ $? -ne 0 ]; then
    echo "Error: Failed to copy pre-commit hook to $DEST_HOOK_PATH"
    exit 1
fi

# Set executable permissions for the pre-commit hook
chmod +x "$DEST_HOOK_PATH"
if [ $? -ne 0 ]; then
    echo "Error: Failed to set executable permissions for $DEST_HOOK_PATH"
    exit 1
fi

echo "Pre-commit hook installed successfully!"
