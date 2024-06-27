import subprocess

def run_gprof(executable):
    # Exécuter le programme pour générer le profil
    subprocess.run([executable])
    
    # Afficher le rapport de profilage et capturer la sortie
    result = subprocess.run(["gprof", executable, "gmon.out"], text=True, capture_output=True)
    print(result.stdout)

run_gprof("./quicksort")