# FileXOR
Applicativo con interfaccia a linea di comando che consente di criptare files attraverso l'One Time Pad (XOR)

## Informazioni generali
**Estensione file cifrati:** .xorenc

**Algoritmo per lo ZEROIZE:** DoD 5220.22-M

# Comandi

**Encrypt:**
- **Sintassi:** ```FileXOR.exe <plainTextFile> <keyFile>```

**Decrypt:**
- **Sintassi:** ```FileXOR.exe <cipherTextFile> <keyFile>```

**Generazione chiavi:**
- **Sintassi:** ```FileXOR.exe keygen <keyFile> <keyLen>```

**Rimozione di N byte dalla chiave:**
- **Sintassi:** ```FileXOR.exe strip <keyFile> <n>```

**ZEROIZE della chiave:**
- **Sintassi:** ```FileXOR.exe zeroize <keyFile>```


# Utilizzo corretto
- La chiave deve essere **totalmente random** (ripetizioni di parti della chiave non ammesse!)
- **LA CHIAVE NON DEVE MAI ESSERE RIUTILIZZATA** (Consigliato distruggere la porzione utilizzata)
- **ATTENZIONE!** Mantenere la chiave al sicuro, non su dispositivi connessi ad internet.
- **ATTENZIONE!** l'opzione keygen utilizza la funzione rand() che non è sicura per applicazioni riguardanti la crittografica, si sconsiglia l'utilizzo di questa funzionalità se non per scopi di test.


# Disclaimer
Tale implementazione è a puro scopo di divertimento, non deve essere presa in considerazione come punto di riferimento.

Non mi assumo nessuna responsabilità di eventuali danni provocati da questo codice o da suoi possibili usi impropri.

# Sitografia
- https://www.bitraser.com/article/DoD-5220-22-m-standard-for-drive-erasure.php#:~:text=The%20DoD%205220.22%2DM%20algorithm,data%20wipe%20method%20will%20take
