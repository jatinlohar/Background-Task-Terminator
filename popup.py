
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import os
import signal
from PIL import Image, ImageTk
import time



# Function to kill a process by its PID
def kill_process_by_pid(pid):
    try:
        os.kill(pid, signal.SIGTERM)
    except ProcessLookupError:
        messagebox.showinfo("Action", f"Process with PID {pid} not found.")
    except PermissionError:
        messagebox.showinfo("Action", f"Permission denied to kill process with PID {pid}.")

# Function to kill processes with the same command name
def kill_processes_by_cmd(cmd):
    process_info = read_process_info('pids.txt')
    pids_to_kill = [pid for pid, process_name in process_info if process_name == cmd]

    if pids_to_kill:
        for pid in pids_to_kill:
            kill_process_by_pid(pid)
        messagebox.showinfo("Action", f"All processes with command '{cmd}' have been killed.")
    else:
        messagebox.showinfo("Action", f"No processes found with command '{cmd}'.")


    

# Function to read process info from output.txt
def read_process_info(file_path):
    process_info = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        for line in lines[0:]:
            columns = line.split()
            pid = int(columns[1])
            process_name = columns[0]
            process_info.append((pid, process_name))
        return process_info


def ignore_button_action():
    selected_sleep_time = 10
    with open('Makefile', 'r') as makefile:
        lines = makefile.readlines()

    with open('Makefile', 'w') as makefile:
        for line in lines:
            if line.startswith('SLEEP_TIME'):
                makefile.write(f'SLEEP_TIME = {selected_sleep_time}\n')
            else:
                makefile.write(line)

        if not any(line.startswith('SLEEP_TIME') for line in lines):
            makefile.write(f'SLEEP_TIME = {selected_sleep_time}\n')

    popup.destroy()
    # time.sleep(60)
    root.quit()


def write_sleep_time_to_makefile():
    selected_sleep_time = int(sleep_var.get())
    with open('Makefile', 'r') as makefile:
        lines = makefile.readlines()

    with open('Makefile', 'w') as makefile:
        for line in lines:
            if line.startswith('SLEEP_TIME'):
                makefile.write(f'SLEEP_TIME = {selected_sleep_time}\n')
            else:
                makefile.write(line)

        if not any(line.startswith('SLEEP_TIME') for line in lines):
            makefile.write(f'SLEEP_TIME = {selected_sleep_time}\n')

        # makefile.write('\n')
        # makefile.write('sleep:\n')
        # makefile.write(f'\tsleep ${{SLEEP_TIME}}\n')

    popup.destroy()
    root.quit()


# Create the popup window with options to select which process to kill
def create_popup():
    # root = tk.Tk()
    # root.withdraw()  # Hide the main window
    # root.overrideredirect(True)  # Remove window decorations
    # root.attributes("-topmost", True)  # Keep the popup on top

    # # Create a popup window
    # popup = tk.Toplevel(root)
    # popup.title("Speed Up Your Laptop")

    # popup.configure(bg="#D5E1F2")

    # Load the background image
    # bg_image = Image.open("background_image.png")  # Replace with the actual image path
    # bg_photo = ImageTk.PhotoImage(bg_image)

    popup.configure(bg="white")
    
    # Text in the popup
    text_label = tk.Label(popup, text="These process are running. Do you want to close", font=("Arial", 16, "normal"))
    text_label.pack(padx=20, pady=20)
    text_label.config(bg="white")
    # 


    button_width = popup.winfo_reqwidth() - 150 
    # Read process info from output.txt
    file_path = 'pids.txt'
    process_info = read_process_info(file_path)


    if len(process_info) == 0:
        popup.destroy()
        root.quit()
    # Create checkboxes for each unique command
    checkboxes = {}
    for pid, process_name in process_info:

        
        if process_name not in checkboxes:
            checkboxes[process_name] = tk.IntVar()

            #process_name = process_name.upper()
            checkbox = tk.Checkbutton(popup, text=process_name.upper(), variable=checkboxes[process_name], onvalue=1, offvalue=0, selectcolor="#27d11d",font=("Verdana", 12), borderwidth=0.1, relief="ridge",width=button_width, anchor='w',padx=10, indicatoron = False)
            
            # checkbox = ttk.Checkbutton(popup, text=process_name.upper(), variable=checkboxes[process_name], onvalue=1, offvalue=0, cursor='hand2', relief="ridge")
            if checkboxes[process_name].get() == 1:

                checkbox.config(bg="green")
            
            # checkbox.config(bg="") 
            # checkbox.config(bg="green")
            checkbox.pack(padx=10, pady=5)
            # checkbox.pack(fill='x', padx=5)

    button_width = popup.winfo_reqwidth() - 200

    # "Kill Process" button
    def kill_selected_processes():
        selected_processes = [process for process, var in checkboxes.items() if var.get() == 1]
        for cmd in selected_processes:
            kill_processes_by_cmd(cmd)
        popup.destroy()
        root.quit()

    kill_button = tk.Button(popup, text="Kill", command=kill_selected_processes,width=14)
    # kill_button.config(bg="white")
    kill_button.pack(side="left", padx=10, pady=10)

    # "Ignore" button
    ignore_button = tk.Button(popup, text="Ignore", command=ignore_button_action, width=14)
    # ignore_button.config(bg="white")
    ignore_button.pack(side="left", padx=10, pady=10)

    ###########################################33333333
    # Dropdown box for sleep time
    # sleep_label = tk.Label(popup, text="Select sleep time:")
    # sleep_label.pack(padx=20, pady=10)

    sleep_options = [2, 5, 10, 15, 30]  # Available sleep times in minutes
    global sleep_var  # Need to make it global so that it's accessible in write_sleep_time_to_makefile
    sleep_var = tk.IntVar(popup)
    sleep_var.set(sleep_options[0])  # Default value

    sleep_dropdown = tk.OptionMenu(popup, sleep_var, *sleep_options)
    sleep_dropdown.pack(side="right", padx=10,pady=10)

    # Button to write sleep time to Makefile
    sleep_button = tk.Button(popup, text="Set Sleep Time", command=write_sleep_time_to_makefile)
    sleep_button.pack(side="right", padx=10, pady=10)

    # Your existing code here

    # Center the popup on the screen
    popup.update_idletasks()
    popup.geometry(f"+{int((root.winfo_screenwidth() - popup.winfo_reqwidth()) / 2)}+{int((root.winfo_screenheight() - popup.winfo_reqheight()) / 2)}")

    

    root.mainloop()


root = tk.Tk()
root.withdraw()  # Hide the main window
root.overrideredirect(True)  # Remove window decorations
root.attributes("-topmost", True)  # Keep the popup on top


# Create a popup window
popup = tk.Toplevel(root)
popup.title("Speed Up Your Laptop")
# popup.resizable(False, False)

popup.protocol("WM_DELETE_WINDOW", exit)
# Call the create_popup function
create_popup()
# time.sleep(10)

popup.destroy()
root.quit()
