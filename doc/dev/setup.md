# Setup

## Project structure and variables

Execute
```
$ source setup.sh
```
The following environment variables are defined:

* `$DEV`: Root
* `$B`(`build/`): Builder
* `$S`(`src/`): Executable Source
* `$SC`(`src/Client`): Client Source
* `$SS`(`src/Server`): Server Source
* `$L`(`lib/`): Library Source
* `$T`(`test/`): Test
* `$BIN`(`bin/`): Output directory

and we have the aliases:

1. `builder`(`python $B/builder.py`): Build script

	For help on its usage, execute `builder -h`.
2. `client`: Client executable
3. `server`: Server executable

The project structure is as follows:
```
assets/      # Images, texts, etc.
bin/         # A symlink to bin_debug/, bin_exp/, or bin_release
bin_debug/   # Debug outputs
	app/         # Executable
	test/        # Unit test
	deploy/      # Installers
	build/       # Build intermediates
bin_exp/     # Experimental outputs (debug with release flags)
	app/
	test/
	deploy/
	build/
bin_release/ # Release outputs
	app/
	deploy/
	build/
build/       # Make scripts
src/         # Application source
	Client/    # Client code
	Server/    # Server code
lib/         # Library source
	[Library Name]/ # Individual library
test/        # Unit and Integration tests
	lib/
	client/
	server/
```

Executable branches (see [Style](style.md)):

* `client`(`$SC`)
* `server`(`$SS`)
* `viewer`(`$SV`)

Library branches:

* `Ele`(`$SL/Ele`) Game Elements
* `GUI`(`$SL/GUI`) User interface and client graphics
* `Gen`(`$SL/Gen`) World generation
* `IO`(`$SL/IO`) Game saves
* `Net`(`$SL/Net`) Networking
* `Sim`(`$SL/Sim`) Simulation, Physics
* `Sys`(`$SL/Sys`) System calls and platform dependent functionalities

## Builder

The builder has several functionalities:

1. `-p,--proj`: Configure the project
2. `-b,--build`
3. `-t,--test`
3. `-c,--clean`

As an example, to configure the debug environment and build:
```
$ builder -p
$ builder -b
```
Notice that the builder automatically adds in the debug flag `-l Debug`.
