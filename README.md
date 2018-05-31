# Project Dominion

2D Territorial Strategy Game

## Development

To setup development, execute
```
source setup.sh
```

The project structure is as follows:
```
assets/      # Images, texts, etc.
bin_debug/   # Debug outputs
	app/ # Executable
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
	lib/       # Libraries
		[Library Name]/ # Individual library
	client/    # Client code
	server/    # Server code
test/        # Unit and Integration tests
	lib/
	client/
	server/
```

For more information, see [Development](doc/dev)
