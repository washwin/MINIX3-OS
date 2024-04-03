import subprocess
import matplotlib.pyplot as plt

addressable_pages = 60
swap_space_blocks = 60
physical_frames = list(range(10, 51, 10))
policy = ["fifo", "lru", "random"]

def get_page_fault(policy, file, physical_frames):
    command = f"./{policy} {addressable_pages} {physical_frames} {swap_space_blocks} {file}"
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    output_lines = result.stdout.splitlines()
    page_fault= int(output_lines[-1].split()[-1])  # Convert to integer
    return page_fault

def make_plot(file):
    for p in policy:
        page_faults = []  # Reset page_faults for each policy
        for pf in physical_frames:
            page_faults.append(get_page_fault(p, file, pf))
        plt.plot(physical_frames, page_faults, label=p)

    # plt.gca().invert_yaxis()
    plt.xlabel('Physical frames')
    plt.ylabel('Page faults')
    plt.title('Physical frames vs page faults')
    plt.legend()
    plt.savefig(f"plot_{file}.png")
    plt.close()  # Close the plot to clear it for the next iteration

def main():
    for i in range(1, 6):
        make_plot(f"req{i}.dat")

if __name__ == "__main__":
    main()
