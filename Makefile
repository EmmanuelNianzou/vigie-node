# Raccourcis. `make test` suffit pour le coeur C pur ; `make build` demande arduino-cli (facultatif : l'IDE compile aussi).
FQBN ?= esp32:esp32:esp32
.PHONY: test build format lint
test:
	cmake -S test_apps/host_unit -B build-host -DCMAKE_BUILD_TYPE=Debug
	cmake --build build-host
	ctest --test-dir build-host --output-on-failure
build:
	arduino-cli compile --fqbn $(FQBN) --output-dir build vigie_node
format:
	find vigie_node/src test_apps -name '*.[ch]' -print0 | xargs -0 clang-format -i
lint:
	cppcheck --enable=warning,style --error-exitcode=1 --suppress=missingIncludeSystem vigie_node/src
