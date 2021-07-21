
mkdir -p tmp

for TEST_FILE in tests/test*
do
	echo "=== corriendo $TEST_FILE ==="
	valgrind ./agenda_interfaz < $TEST_FILE > /dev/null
	echo ""
done