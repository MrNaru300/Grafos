import subprocess
import csv
import matplotlib.pyplot as plt


# Ask the user if they want to overwrite the benchmark CSV or continue from where it stopped
overwrite_csv = input("Do you want to overwrite the benchmark CSV? (y/n): ")

subprocess.run(["g++", "testes.cpp"])

# Initialize lists to store the benchmark results
execution_times = {"edmonds": [], "gabow": []}

# Open the CSV file in write mode
with open('benchmark_results.csv', 'w' if overwrite_csv.lower() == 'y' else 'a', newline='') as csvfile:
    writer = csv.writer(csvfile)
    
    # Write header if the file is empty
    if csvfile.tell() == 0:
        writer.writerow(['Number of Nodes', 'Edmonds', 'Gabow'])

    # Ask the user for the range of number of nodes
    num_nodes_range = range(int(input("Enter the starting number of nodes: ")), int(input("Enter the ending number of nodes: ")) + 1)


    # Iterate over the number of nodes
    for num_nodes in num_nodes_range:

        print("-------------------------")
        print("Number of nodes: ", num_nodes)

        # Run the subprocess and capture the output
        output = None
        while output is None:
            try:
                output = subprocess.check_output(["./a.out", str(num_nodes), "10"], timeout=300)
            except subprocess.TimeoutExpired:
                continue
            break


        # Parse the output to extract execution time
        execution_time_edmonds = float(output.decode().split()[2])

        print("Edmonds: ", execution_time_edmonds)

        # Parse the output to extract execution time
        execution_time_gabow = float(output.decode().split()[5])

        print("Gabow: ", execution_time_gabow)

        # Write the benchmark results to the CSV file
        writer.writerow([num_nodes, execution_time_edmonds, execution_time_gabow])

