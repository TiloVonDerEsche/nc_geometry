import subprocess
import os
import csv

def run_cmd(command, working_directory=None):
    """
    Execute a terminal command and return its output.

    Args:
        command (str): The terminal command to execute (e.g., "gcc main.c -o main.exe").
        working_directory (str, optional): Directory to run the command in. Defaults to current directory.

    Returns:
        tuple: (success (bool), output (str)) indicating if command succeeded and its output/error.
    """
    try:
        # Set the working directory if provided, otherwise use current directory
        cwd = working_directory if working_directory else os.getcwd()

        # Run the command
        result = subprocess.run(
            command,
            shell=True,  # Required for commands with spaces like "gcc main.c -o main.exe"
            cwd=cwd,     # Set working directory
            capture_output=True,  # Capture stdout and stderr
            text=True     # Return output as string instead of bytes
        )

        # Check if command was successful
        if result.returncode == 0:
            return True, result.stdout
        else:
            return False, result.stderr

    except subprocess.SubprocessError as e:
        return False, f"Error executing command: {str(e)}"

# Example usage
if __name__ == "__main__":
    #working_directory=""
    success, message = run_cmd("gcc main.c -o main.exe")
    print(f"gcc compiled with message: \n{message}")

    success, message = run_cmd(".\\main.exe")
    print(f"Ran main.exe with message: \n{message}")

    #read output csv
    with open(".\\data\\track_list.csv", mode='r', encoding='utf-8') as file:
        csv_reader = csv.reader(file)

        #remove header
        header = next(csv_reader, None)
        if header:
            print("Header:", header)

        for row in csv_reader:
            print(row)
