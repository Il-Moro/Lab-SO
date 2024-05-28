#!/bin/bash

#quindi: ho due argomemti
    # 1) un file: devo leggere il file riga per riga; ogni riga contiene
    #   un path che devo aggiungere al FS del container
    # 2) è un comando: un file eseguibile che deve essere incluso nei 
    #   file inseriti all'interno del FS montato per il container

# Controlla che il primo argomento sia un file
#!/bin/bash

tmp="/tmp/lab_5"

# Verifica che ci siano esattamente due argomenti
if [[ $# -ne 2 ]]; then
    echo "Uso: $0 <file_di_configurazione> <comando>"
    exit 1
fi

# Assegna gli argomenti a variabili
config_file=$1
command=$2

# Controlla che il primo argomento sia un file
if [[ -f $config_file ]]; then

    # Crea la directory temporanea se non esiste già
    mkdir -p "$tmp"

    # Legge il file riga per riga
    while IFS= read -r riga; do
        read -r path_1 path_2 <<< "$riga"
        path_2="$tmp$path_2"
        
        # Controlla se path_1 è una directory
        if [[ -d $path_1 ]]; then 
            mkdir -p "$path_2" # Crea la directory di destinazione
            bindfs --no-allow-other "$path_1" "$path_2"
            if [[ $? -ne 0 ]]; then
                echo "Errore nel montaggio di $path_1 su $path_2"
            fi
        
        # Controlla se path_1 è un file regolare o un character device
        elif [[ -f $path_1 || -c $path_1 ]]; then
            mkdir -p "$(dirname "$path_2")" # Crea le directory necessarie
            cp "$path_1" "$path_2"
        
        # Gestisce altri tipi di file, se necessario
        else
            echo "Percorso non valido o tipo di file non supportato: $path_1"
        fi
    done < "$config_file"

    # Esegue il comando in ambiente fakechroot
    fakechroot chroot "$tmp" "$command"

    # Smonta tutti i bind mounts e pulisce la directory temporanea
    # echo "Smontaggio dei bind mounts e pulizia della directory temporanea"
    # find "$tmp" -type d | sort -r | xargs -I {} umount {}
    # rm -rf "$tmp"

else
    echo "Il primo argomento deve essere un file."
    exit 1
fi



# chroot /path/to/new/root command