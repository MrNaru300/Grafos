import csv
import matplotlib.pyplot as plt

# Initialize the execution times dictionary
execution_times = {"edmonds": [], "gabow": []}
num_nodes_range = []
nodes_limit = 2500

# Open the CSV file in read mode
with open('benchmark_results.csv', 'r', newline='') as csvfile:
    reader = csv.reader(csvfile)

    # Skip the header
    next(reader)

    # Iterate over the rows
    for row in reader:
        # Extract the benchmark results
        num_nodes = int(row[0])
        if num_nodes > nodes_limit:
            break
        execution_time_edmonds = float(row[1])
        execution_time_gabow = float(row[2])

        # Store the benchmark results in the lists
        execution_times["edmonds"].append(execution_time_edmonds*1000)
        execution_times["gabow"].append(execution_time_gabow*1000)
        num_nodes_range.append(num_nodes)

# Plot the benchmark results
plt.plot(num_nodes_range, execution_times["edmonds"], label="Edmonds")
plt.plot(num_nodes_range, execution_times["gabow"], label="Gabow")
plt.title("Benchmark Results")
plt.xlabel("Number of Nodes")
plt.ylabel("Execution Time (milliseconds)")
plt.ylim(bottom=0)
plt.grid()
plt.legend()
plt.show()
