import pandas as pd
import matplotlib.pyplot as plt

# Load data
filename = "PopulacaoInicialAleatoria/resultados3(20itens).txt"  # Update with the correct file path if needed
data = pd.read_csv(filename, sep="|", header=None)

# Rename columns for easier access
data.columns = [
    "Best Fitness",
    "Sum of Values (Best)",
    "Sum of Weights (Best)",
    "Iteration Found",
    "Last Gen Value and Weight",
]

# Separate "Last Gen Value and Weight" into two columns
data[["Last Gen Value", "Last Gen Weight"]] = (
    data["Last Gen Value and Weight"].str.split(",", expand=True).astype(float)
)
data.drop(columns=["Last Gen Value and Weight"], inplace=True)

# Convert all columns to numeric for analysis
data = data.apply(pd.to_numeric, errors="coerce")

# Normalize each metric (except "Iteration Found") by dividing by the maximum and multiplying by 100
data["Best Fitness (%)"] = (data["Best Fitness"] / data["Best Fitness"].max()) * 100
data["Sum of Values (Best) (%)"] = (
    data["Sum of Values (Best)"] / data["Sum of Values (Best)"].max()
) * 100
data["Sum of Weights (Best) (%)"] = (
    data["Sum of Weights (Best)"] / data["Sum of Weights (Best)"].max()
) * 100

# Plot histograms
plt.figure(figsize=(10, 8))

# Best Fitness histogram (normalized)
plt.subplot(2, 2, 1)
plt.hist(data["Best Fitness (%)"], bins=40, edgecolor="black")
plt.title("Histograma do Melhor Fitness")
plt.xlabel("Melhor Fitness (%)")
plt.ylabel("Frequência")

# Sum of Values (Best) histogram (normalized)
plt.subplot(2, 2, 2)
plt.hist(data["Sum of Values (Best) (%)"], bins=40, edgecolor="black")
plt.title("Soma dos valores da melhor solução")
plt.xlabel("Valores da melhor solução (%)")
plt.ylabel("Frequência")

# Sum of Weights (Best) histogram (normalized)
plt.subplot(2, 2, 3)
plt.hist(data["Sum of Weights (Best) (%)"], bins=40, edgecolor="black")
plt.title("Soma dos pesos da melhor solução")
plt.xlabel("Pesos da melhor solução (%)")
plt.ylabel("Frequência")

# Iteration Found histogram (not normalized)
plt.subplot(2, 2, 4)
plt.hist(data["Iteration Found"], bins=40, edgecolor="black")
plt.title("Iteração em que a melhor solução foi encontrada")
plt.xlabel("Iteração")
plt.ylabel("Frequência")

# Adjust layout and display the plots
plt.tight_layout()
plt.show()
