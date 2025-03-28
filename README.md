# ipc-hospital-app

## Hospital Workflow Simulation

This project simulates a basic hospital workflow using System V message queues for inter-process communication. It consists of three processes:

Registry: Registers a patient with their details, including diagnosis and initial prognosis.

Chief Doctor: Receives the patient data, reviews the diagnosis, and randomly determines a good or bad prognosis. If the prognosis is bad, the patient’s data is forwarded.

Underworld: Represents an alternative process that handles patients with a bad prognosis.

The simulation uses message queues (with unique IDs for the registry and underworld) to pass patient information between these processes.
