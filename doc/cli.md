# ![](logo.png) [[FORTH/CLI|CLI]] 0.0.1
## Command Line Interface ([[lang/REPL|REPL]])

(c) Dmitry Ponyatov <<dponyatov@gmail.com>> 2025 MIT

github: https://github.com/ponyatov/cli

- based on [[bcx]] bytecode interpreter and
	- [[cli/FORTH|FORTH]] script language model
- focused on IoT and [[em/MCU|MCU]] devices
	- and embedded [[Linux/Linux|Linux]]
	- virtual memory limited to 64K (16-bit code & data addressing)
	- 32-bit [[data stack]] `D`
	- [[return stack]] `R` limited to 255 cells
- optional AOT compilation:
	- compiled [[vm/bytecode|bytecode]] can be targeted only for execution
		- no interactive shell & on-device compiler
	- so some parts can be removed from compiled [[bcx/bytecode|bytecode]] image
		- optional [[vocabulary]]
		- [[cli/syntax#parser]]
