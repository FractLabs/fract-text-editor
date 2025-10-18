import sys
import os
import tkinter as tk
from tkinter import filedialog, messagebox

APP_TITLE = "FractTextEditor"

class FractTextEditor(tk.Tk):
    def __init__(self, initial_path=None):
        super().__init__()

        self.title(APP_TITLE)
        self.geometry("900x600")
        self.minsize(600, 400)
        self.current_file = None
        self._is_modified = False

        self.bg_color = "#1e1f22"
        self.panel_color = "#2b2b2f"
        self.btn_color = "#3a3a3f"
        self.text_bg = "#121213"
        self.text_fg = "#e6e6e6"
        self.gutter_bg = "#111214"
        self.gutter_fg = "#9a9a9a"

        self.configure(bg=self.bg_color)

        toolbar = tk.Frame(self, bg=self.panel_color, pady=6)
        toolbar.pack(fill="x", side="top")

        self.btn_new = tk.Button(toolbar, text="New", command=self.new_file, bg=self.btn_color, fg="white", relief="flat", padx=8, pady=4)
        self.btn_open = tk.Button(toolbar, text="Open", command=self.open_file_dialog, bg=self.btn_color, fg="white", relief="flat", padx=8, pady=4)
        self.btn_save = tk.Button(toolbar, text="Save", command=self.save_file, bg=self.btn_color, fg="white", relief="flat", padx=8, pady=4)
        self.btn_close = tk.Button(toolbar, text="Close", command=self.close_file, bg=self.btn_color, fg="white", relief="flat", padx=8, pady=4)

        self.btn_new.pack(side="left", padx=(10,4))
        self.btn_open.pack(side="left", padx=4)

        self.btn_save.pack_forget()
        self.btn_close.pack_forget()

        self.content_frame = tk.Frame(self, bg=self.bg_color)
        self.content_frame.pack(fill="both", expand=True)

        self.welcome_label = tk.Label(self.content_frame, text="Welcome to Fract Text Editor", bg=self.bg_color, fg=self.text_fg, font=("Segoe UI", 20))
        self.welcome_label.place(relx=0.5, rely=0.4, anchor="center")

        self.editor_frame = tk.Frame(self.content_frame, bg=self.bg_color)
        self.line_numbers = tk.Text(self.editor_frame, width=4, padx=4, pady=4, takefocus=0, bd=0, bg=self.gutter_bg,
                                    fg=self.gutter_fg, state="disabled", wrap="none", relief="flat")
        self.text = tk.Text(self.editor_frame, bg=self.text_bg, fg=self.text_fg, insertbackground=self.text_fg,
                            undo=True, wrap="word", padx=6, pady=6, relief="flat")
        self.scrollbar = tk.Scrollbar(self.editor_frame, command=self._on_scrollbar)
        self.text.configure(yscrollcommand=self._on_text_scroll)

        self.text.bind("<Key>", self._on_user_edit)
        self.text.bind("<BackSpace>", self._on_user_edit)
        self.text.bind("<Delete>", self._on_user_edit)
        self.text.bind("<Control-v>", self._on_user_edit)
        self.text.bind("<Control-V>", self._on_user_edit)
        self.text.bind("<Button-2>", self._on_user_edit)
        self.text.bind("<KeyRelease>", lambda e: self._update_line_numbers())
        self.text.bind("<ButtonRelease-1>", lambda e: self._update_line_numbers())
        self.text.bind("<MouseWheel>", lambda e: self._update_line_numbers())
        self.text.bind("<Configure>", lambda e: self._update_line_numbers())

        try:
            self.text.bind("<<Modified>>", self._on_text_modified)
        except Exception:
            pass

        self.bind_all("<Control-n>", lambda e: self.new_file())
        self.bind_all("<Control-o>", lambda e: self.open_file_dialog())
        self.bind_all("<Control-s>", lambda e: self.save_file())
        self.bind_all("<Control-w>", lambda e: self.close_file())
        self.bind_all("<Control-q>", lambda e: self.on_quit())

        self.protocol("WM_DELETE_WINDOW", self.on_quit)

        if initial_path:
            if os.path.exists(initial_path) and os.path.isfile(initial_path):
                try:
                    self.open_file(initial_path)
                except Exception as exc:
                    messagebox.showwarning("Error opening file", f"Could not open '{initial_path}':\n{exc}")
            else:
                messagebox.showwarning("File not found", f"Path provided not found or not a file:\n{initial_path}")

        self._update_title()

    def _update_title(self):
        name = os.path.basename(self.current_file) if self.current_file else "Untitled"
        mod = "*" if self._is_modified else ""
        self.title(f"{APP_TITLE} — {name}{mod}")

    def _on_text_modified(self, event=None):
        self._set_modified(True)

        try:
            self.text.edit_modified(False)
        except Exception:
            pass

    def _on_user_edit(self, event=None):
        self._set_modified(True)
        return None

    def _set_modified(self, val: bool):
        if self._is_modified == val:
            return
        self._is_modified = val
        self._update_title()

    def _on_scrollbar(self, *args):
        self.text.yview(*args)
        self.line_numbers.yview(*args)

    def _on_text_scroll(self, *args):
        try:
            self.scrollbar.set(*args)
            self.line_numbers.yview_moveto(float(args[0]))
        except Exception:
            pass

    def _update_line_numbers(self):
        try:
            last_line = int(self.text.index('end-1c').split('.')[0])
        except Exception:
            last_line = 1
        width = max(2, len(str(last_line)))
        self.line_numbers.configure(width=width)
        lines = "\n".join(str(i) for i in range(1, last_line + 1))
        self.line_numbers.config(state="normal")
        self.line_numbers.delete("1.0", "end")
        self.line_numbers.insert("1.0", lines)
        self.line_numbers.config(state="disabled")

    def _show_editor(self):
        self.welcome_label.place_forget()
        self.editor_frame.pack(fill="both", expand=True)
        self.line_numbers.pack(side="left", fill="y")
        self.scrollbar.pack(side="right", fill="y")
        self.text.pack(fill="both", expand=True, side="left")
        if not self.btn_save.winfo_ismapped():
            self.btn_save.pack(side="left", padx=4)
        if not self.btn_close.winfo_ismapped():
            self.btn_close.pack(side="left", padx=4)
        self._update_line_numbers()

    def _show_welcome(self):
        self.text.pack_forget()
        self.scrollbar.pack_forget()
        self.line_numbers.pack_forget()
        self.editor_frame.pack_forget()
        self.welcome_label.place(relx=0.5, rely=0.4, anchor="center")
        if self.btn_save.winfo_ismapped():
            self.btn_save.pack_forget()
        if self.btn_close.winfo_ismapped():
            self.btn_close.pack_forget()
        self.current_file = None
        self._set_modified(False)

    def _ask_save_discard_cancel(self):
        resp = messagebox.askyesnocancel("Save changes?", "Do you want to save changes to the current file?")
        if resp is True:
            return "save"
        if resp is False:
            return "dont_save"
        return "cancel"

    def _confirm_save_if_modified(self):
        if self._is_modified:
            ans = self._ask_save_discard_cancel()
            if ans == "cancel":
                return False
            if ans == "save":
                if not self.save_file():
                    return False
        return True

    # Actions
    def new_file(self):
        if not self._confirm_save_if_modified():
            return
        self.current_file = None

        self.text.delete("1.0", "end")
        self._set_modified(False)
        self._show_editor()

    def open_file_dialog(self):
        if not self._confirm_save_if_modified():
            return
        path = filedialog.askopenfilename(title="Open file", filetypes=[("Text files", "*.txt"), ("All files", "*.*")])
        if path:
            self.open_file(path)

    def open_file(self, path):
        try:
            with open(path, "r", encoding="utf-8") as f:
                data = f.read()
        except Exception as exc:
            messagebox.showerror("Open error", f"Failed to open file:\n{exc}")
            return False

        self.text.delete("1.0", "end")
        self.text.insert("1.0", data)
        self.current_file = path
        self._set_modified(False)
        self._show_editor()
        return True

    def save_file(self):
        if not self.current_file:
            path = filedialog.asksaveasfilename(title="Save file", defaultextension=".txt",
                                                filetypes=[("Text files", "*.txt"), ("All files", "*.*")])
            if not path:
                return False
            self.current_file = path
        try:
            with open(self.current_file, "w", encoding="utf-8") as f:
                text = self.text.get("1.0", "end-1c")
                f.write(text)
        except Exception as exc:
            messagebox.showerror("Save error", f"Failed to save file:\n{exc}")
            return False
        self._set_modified(False)
        return True

    def close_file(self):
        if not self._confirm_save_if_modified():
            return
        self._show_welcome()

    def on_quit(self):
        if not self._confirm_save_if_modified():
            return
        self.destroy()


if __name__ == "__main__":
    initial = sys.argv[1] if len(sys.argv) > 1 else None
    app = FractTextEditor(initial_path=initial)
    app.mainloop()
