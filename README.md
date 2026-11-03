# Emergency-Document-Expiry-Renewal-Pipeline
# Emergency Document Expiry & Renewal Pipeline

A lightweight, terminal-based utility written in C designed to track, audit, and prioritize document renewals (e.g., Vehicle Insurance, Pollution Under Control (PUC) certificates, Passports, Driving Licenses, and Policy Agreements). 

The program calculates real-time day buffers and automatically ranks items by urgency to prevent legal non-compliance and missed deadline penalties.

---

## The Problem

Every household manages numerous essential documents with mismatched validity cycles. People often miss renewals—realizing their PUC is expired during a traffic stop, or finding an expired passport when an urgent trip arises. Existing apps either demand invasive cloud/bank access or are cluttered with advertisements. 

This project provides a privacy-first, zero-dependency offline command-line interface (CLI) to track and prioritize renewals directly from the terminal.

---

## Key Features

- **Julian Day Date Engine**: Computes exact day differences between arbitrary calendar dates without external datetime dependencies or operating system libraries.
- **Dynamic Array Allocation**: Uses heap memory (`malloc`, `realloc`, and `free`) for parallel arrays, allowing dynamic scaling of records during runtime.
- **Urgency Pipeline Classification**:
  - `[EXPIRED / BREACH]`: Days left $\le 0$
  - `[ACTION REQUIRED]`: Days left $\le$ Required Renewal Lead Time
  - `[VALID / SAFE]`: Days left $>$ Required Renewal Lead Time
- **Recursive QuickSort Engine**: Ranks all documents in ascending order of days remaining so that impending deadlines always float to the top.
- **Memory Safety**: Clean deallocation of all dynamically sized string buffers and array handles on exit to avoid memory leaks.

---

## Technical Stack & Concepts Used

- **Language**: C (Standard C99 / C11)
- **Memory Management**: Pointers, pointer indirection (`char***` dynamic arrays), dynamic reallocation (`realloc`), dynamic heap assignment (`malloc`), and deallocation (`free`).
- **Data Structures**: Parallel dynamically allocated arrays.
- **Algorithms**: Recursive QuickSort (Divide and Conquer), Julian Day Number computation.
- **Modularity**: Separation of concerns into input handling, mathematical date conversion, record swapping, recursive sorting, and tabular rendering.

---

## Project Structure

```text
├── main.c        # Core C application source code
├── README.md     # Project documentation
└── .gitignore    # Ignores compiled binaries (*.exe, *.o, *.out)
<img width="683" height="481" alt="image" src="https://github.com/user-attachments/assets/77415bac-405e-41c6-8b95-a5a940c992b0" />
<img width="635" height="456" alt="image" src="https://github.com/user-attachments/assets/02147db5-5cce-442d-8cb9-4b2a340245e4" />
<img width="695" height="252" alt="image" src="https://github.com/user-attachments/assets/bce09627-ca2e-4200-b212-826f41f3aa46" />
<img width="695" height="252" alt="image" src="https://github.com/user-attachments/assets/930bbb2b-34da-4b6a-88aa-c81dad69d970" />
<img width="720" height="470" alt="image" src="https://github.com/user-attachments/assets/c9d76611-14bc-4f93-8191-181bcb431165" />
<img width="720" height="470" alt="image" src="https://github.com/user-attachments/assets/7781ad24-a729-44fb-acd9-e7524b92a325" />
