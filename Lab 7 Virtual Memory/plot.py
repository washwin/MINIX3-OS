import subprocess
import matplotlib.pyplot as plt

def run_command(s, l):
    command = f"python3 relocation.py -s {s} -n 100 -l {l} -c"
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    output_lines = result.stdout.splitlines()
    fraction = float(output_lines[-1])  # Assuming the fraction is printed on the last line
    return fraction

def generate_command_combinations():
    s_values = [0, 1, 2]
    l_values = range(0, 1001, 200)
    return [(s, l) for s in s_values for l in l_values]

def main():
    command_combinations = generate_command_combinations()

    results = {s: [] for s in range(0, 3)}

    for s, l in command_combinations:
        result = run_command(s, l)
        results[s].append((l, result))

    for s, data in results.items():
        x = [item[0] for item in data]
        y = [item[1] for item in data]
        plt.plot(x, y, label=f"seed {s}")

    plt.xlabel('Limit')
    plt.ylabel('Valid fraction')
    plt.title('Valid fraction vs Limit')
    plt.legend()
    plt.savefig("plot.png")
    # plt.show()

if __name__ == "__main__":
    main()
