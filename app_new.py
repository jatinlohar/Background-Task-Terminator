import sys
from tkinter import messagebox
import time
import tkinter as tk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

root = tk.Tk()
root.title("Tkinter Pie Chart Example")

canvas = tk.Canvas(root, width=1200, height=600)
canvas.pack()

# def show_popup(process_name):
#     popup = tk.Tk()
#     popup.wm_title("Clear Process")
#     popup.geometry("300x100")
#     label = tk.Label(popup, text=f"The process '{process_name}' is least frequent. Do you want to clear it?")
#     label.pack(side="top", fill="x", pady=10)

#     B1 = tk.Button(popup, text="Yes", command = lambda: clear_process(process_name, popup))
#     B1.pack()
#     B2 = tk.Button(popup, text="No", command = popup.destroy)
#     B2.pack()

#     popup.mainloop()

# def clear_process(process_name, popup):
#     print(f"The process '{process_name}' will be cleared.")
#     popup.destroy()


def create_pie_charts():
    data = []
    with open('output.txt', 'r') as file:
        next(file)
        for line in file:
            parts = line.split()
            pid, cpu, rss, cmd = int(parts[0]), float(parts[1]), int(parts[2]), parts[3] 
            data.append((cmd, cpu, rss))

    cmd_counts = {}  # To handle multiple processes with the same name
    cmds = []
    cpus = []
    rss = []

    for entry in data:
        cmd, cpu, rs = entry[0], entry[1], entry[2]
        if cmd in cmd_counts:
            cmd_counts[cmd][0] += cpu
            cmd_counts[cmd][1] += rs
        else:
            cmd_counts[cmd] = [cpu, rs]
            cmds.append(cmd)

    for cmd in cmds:
        cpus.append(cmd_counts[cmd][0])
        rss.append(cmd_counts[cmd][1])


    # flag = 0
    # temp_x = 0.0
    # for i in cpus:
    #     temp_x += i
    
    # if 100.0-temp_x>0:
    #     cpus.append(100.0 - temp_x)
    #     flag = 1 

    # temp_y = 0
    # for j in rss:
    #     temp_y += j
    # if(8000000-temp_y>0):
    #     rss.append(8000000 - temp_y)
        

    # if (flag==1):
    #     cmds.append('Others')

    #     cmd_counts['Others'] = [100.0-temp_x,8000000-temp_y]

    # fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 5))
    
    # colors = plt.cm.Paired(range(len(cmds)))

    # ax1.pie(cpus, labels=cmds, autopct='%1.1f%%', startangle=90, colors=colors)
    # ax1.axis('equal')
    # ax1.set_title('CPU Usage by Process')

    # ax2.pie(rss, labels=cmds, autopct='%1.1f%%', startangle=90, colors=colors)
    # ax2.axis('equal')
    # ax2.set_title('RSS Usage by Process')

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 5))
    
    colors = plt.cm.Paired(range(len(cmds)))

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 5))
    colors = plt.cm.Paired(range(len(cmds)))

    ax1.pie(cpus, autopct='%1.1f%%', startangle=90, colors=colors)
    ax1.axis('equal')
    ax1.set_title('CPU Usage by Process')

    ax2.pie(rss, autopct='%1.1f%%', startangle=90, colors=colors)
    ax2.axis('equal')
    ax2.set_title('RSS Usage by Process')

    # Embed the pie charts in the Tkinter GUI
    chart1 = FigureCanvasTkAgg(fig, canvas)
    chart1.get_tk_widget().pack(side=tk.LEFT, padx=20)  # Increase the padx value for separation
    
    
    
    # # Create legend
    # legend_frame = tk.Frame(root)
    # legend_frame.pack(side=tk.LEFT, padx=20)

    # legend_frame_label = tk.Label(legend_frame, text="Legend", font=("Helvetica", 16))
    # legend_frame_label.pack()

    # for i, (cmd, color) in enumerate(zip(cmds, colors)):
    #     legend_label = tk.Label(legend_frame, text=f"{cmd} - {cpus[i]:.1f}%", anchor=tk.W)
    #     legend_label.pack()

    #     legend_label_color = tk.Label(legend_frame, text="popup", bg=color)
    #     legend_label_color.pack(side=tk.LEFT)

    root.after(15000, root.quit)

# Center the "Create Pie Charts" button
chart_button = tk.Button(root, text="Create Pie Charts", command=create_pie_charts)
chart_button.pack(pady=20)  # Increase pady value for vertical centering


root.mainloop()

# # Example usage:
# least_frequent_process = "example_process"
# time.sleep(10)  # Wait for 30 seconds
# show_popup(least_frequent_process)



# Check if the tkinter window has been closed
if not root.winfo_exists():
    sys.exit()

    




