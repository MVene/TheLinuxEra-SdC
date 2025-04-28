# TP03: Modo Real vs. Modo Protegido

## 🎯 Objetivos

- completar
- 
- 

## 🔶 Desafíos

### ⚙️ UEFI y Coreboot

#### ¿Qué es UEFI? ¿Cómo puedo usarlo? Mencionar además una función a la que podría llamar usando esa dinámica.


#### ¿Menciona casos de bugs de UEFI que puedan ser explotados?


#### ¿Qué es Converged Security and Management Engine (CSME), the Intel Management Engine BIOS Extension (Intel MEBx)?


#### ¿Qué es coreboot? ¿Qué productos lo incorporan? ¿Cuáles son las ventajas de su utilización?

---

### 🔗 Linker

#### ¿Qué es un linker? ¿Qué hace? 


#### ¿Qué es la dirección que aparece en el script del linker? ¿Por qué es necesaria?


#### Compare la salida de ``objdump`` con ``hd``, verifique dónde fue colocado el programa dentro de la imagen. 


#### Grabar la imagen en un pendrive, probarla en una PC y subir una foto.


#### ¿Para qué se utiliza la opción ``--oformat binary``  en el linker?

---

### 🔒 Modo Protegido

#### Crear un código assembler que pueda pasar a modo protegido (sin macros).


#### ¿Cómo sería un programa que tenga dos descriptores de memoria diferentes, uno para cada segmento (código y datos) en espacios de memoria diferenciados?


#### Cambiar los bits de acceso del segmento de datos para que sea de solo lectura. Intentar escribir, ¿qué sucede?, ¿qué debería suceder a continuación? (revisar el teórico). Verificarlo con gdb.


#### En modo protegido, ¿con qué valor se cargan los registros de segmento?, ¿por qué? 
