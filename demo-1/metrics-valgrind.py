import subprocess

def run_valgrind(executable):
    command = ["valgrind", "--tool=massif", "--massif-out-file=massif.out", executable]
    subprocess.run(command)
    # Lire et afficher le sommaire de l'utilisation de la mémoire
    with open("massif.out", "r") as file:
        for line in file:
            if "mem_heap_B=" in line:
                print(line.strip())

executable = "./quicksort"
run_valgrind(executable)