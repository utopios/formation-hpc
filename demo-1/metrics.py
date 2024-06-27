import subprocess
import time

def run_program(n_runs):
    executable = "./quicksort"
    times = []

    for i in range(n_runs):
        start_time = time.time()
        # Lancer le programme en C
        result = subprocess.run([executable], capture_output=True, text=True)
        end_time = time.time()

        # Calculer le temps d'exécution
        elapsed_time = end_time - start_time
        times.append(elapsed_time)

        # Afficher la sortie du programme et le temps d'exécution
        print(f"Execution {i+1}:")
        print(result.stdout)
        print(f"Temps d'exécution: {elapsed_time:.4f} secondes\n")

    return times

def calculate_average(times):
    # Calculer la moyenne des temps d'exécution
    average_time = sum(times) / len(times)
    return average_time

# Configurer le nombre de fois que le programme doit être exécuté
n_runs = 5
times = run_program(n_runs)

# Calculer et afficher la moyenne des temps d'exécution
average_time = calculate_average(times)
print(f"Temps moyen d'exécution pour {n_runs} essais: {average_time:.4f} secondes")