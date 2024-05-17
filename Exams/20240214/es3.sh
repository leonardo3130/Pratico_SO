#!/bin/bash

# Imposta la directory di lavoro
directory="${1:-.}"

# Verifica se la directory esiste
if [ ! -d "$directory" ]; then
    echo "Errore: La directory specificata non esiste."
    exit 1
fi

# Crea un array associativo per le categorie
declare -A catalogo

# Ciclo sui file nella directory
for file in "$directory"/*; do
    # Ottieni la stringa ritornata dal comando 'file'
    tipo=$(file -b "$file")
    # Aggiungi il file alla categoria corrispondente nel catalogo
    catalogo["$tipo"]+="$(basename "$file")"$'\n'
done

# Stampa il catalogo ordinato per categorie
for tipo in "${!catalogo[@]}"; do
    echo "Categoria: $tipo"
    echo "----------"
    echo -e "${catalogo[$tipo]}"
done
