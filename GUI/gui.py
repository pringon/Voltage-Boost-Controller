from tkinter import *
from ctypes import *
from time import sleep
com_interface = CDLL("../CommunicationInterface_PythonExtended/libcom_interface.so")
com_interface.read_voltage.restype = c_double

class BoostConverter:

    def __init__(self, master):
        self.master = master
        master.title("Boost Converter")

        self.target_voltage = 15
        self.current_voltage = 0
        self.voltage_choice = 0

        self.target_voltage_text = IntVar()
        self.target_voltage_text.set(self.target_voltage)
        self.target_voltage_label = Label(master, textvariable=self.target_voltage_text)
        self.target_voltage_description = Label(master, text="Target Voltage:")

        self.current_voltage_text = IntVar()
        self.current_voltage_text.set(self.current_voltage)
        self.current_voltage_label = Label(master, textvariable=self.current_voltage_text)
        self.current_voltage_description = Label(master, text="Current Voltage:")

        vcmd = master.register(self.validate)
        self.set_voltage_entry = Entry(master, validate="key", validatecommand=(vcmd, "%P"))
        self.set_voltage_entry.bind('<Return>', self.set_voltage)
        self.set_voltage_button = Button(master, text="Set Voltage", command=self.set_voltage)

        #LAYOUT

        #Target voltage label
        self.target_voltage_description.grid(row=0, column=0)
        self.target_voltage_label.grid(row=0, column=1)

        #Current voltage label
        self.current_voltage_description.grid(row=0, column=2)
        self.current_voltage_label.grid(row=0, column=3)

        self.set_voltage_entry.grid(row=1, column=0, columnspan=3, sticky=W+E)
        self.set_voltage_button.grid(row=1, column=3)

    def validate(self, new_text):
        if not new_text:
            self.voltage_choice = 0
            return True

        try:
            if float(new_text) <= 15:
                self.voltage_choice = round(float(new_text), 1)
                return True
        except ValueError:
            return False
        return False

    def set_voltage(self, event=None):
        self.target_voltage = self.voltage_choice
        self.target_voltage_text.set(self.target_voltage)
        com_interface.write_voltage(c_double(self.target_voltage))
        self.set_voltage_entry.delete(0, END)

    def get_voltage(self) :
        self.current_voltage = com_interface.read_voltage()
        self.current_voltage_text.set(self.current_voltage)
        self.master.after(1000, self.get_voltage)

root = Tk()
boost_converter = BoostConverter(root)
com_interface.com_init()
sleep(5)
boost_converter.get_voltage()
root.mainloop()
