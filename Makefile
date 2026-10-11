# Raccourcis : une seule commande pour vérifier avant de commiter.
.PHONY: build flash monitor test format lint
build:
	idf.py build
flash:
	idf.py flash monitor
monitor:
	idf.py monitor
test:
	cmake -S test_apps/host_unit -B build-host -DCMAKE_BUILD_TYPE=Debug
	cmake --build build-host
	ctest --test-dir build-host --output-on-failure
format:
	find components main test_apps -name '*.[ch]' -print0 | xargs -0 clang-format -i
lint:
	cppcheck --enable=warning,style --error-exitcode=1 -I components --suppress=missingIncludeSystem components main
