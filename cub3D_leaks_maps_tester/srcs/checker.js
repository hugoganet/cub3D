const exec = require('util').promisify(require('child_process').exec);
require('dotenv').config();
const { VALGRIND, EXEC, GREP, GREP2} = process.env;
const { printTitle, printRed, printGreen, printBasic } = require('./utility');
const { parserCount, leaksCount } = require('./report');
const progressBar = require('./progressBar');

const makeHeader = (file, maps, index) => {
	printTitle(file);
	progressBar.start(maps.length, index + 1);
	printBasic("\n");
}

const checkParser = async file => {
	try {
		// Execute the command and expect it to fail for invalid maps
		const { stdout, stderr } = await exec(EXEC+file);
		// If we reach here, the command succeeded (exit code 0)
		// For invalid maps, this might be unexpected
		const combinedOutput = stdout + stderr;
		if (combinedOutput.length && (combinedOutput.includes("Error\n") || combinedOutput.includes("Error:"))) {
			printBasic(combinedOutput, '\n');
			printGreen("[OK]", "PARSER : ");
			parserCount.passed++;
		} else {
			printBasic("\n");
			printRed("[FAILED]", "PARSER : ", " ⚠️ Expected error but got success ⚠️");
			printBasic("[n/a]","LEAKS  :")
			parserCount.failed++;
			parserCount.maps.push(file);
		}
	} catch (error) {
		// The command failed (non-zero exit code), which is expected for invalid maps
		const stdout = error.stdout || '';
		const stderr = error.stderr || '';
		const combinedOutput = stdout + stderr;
		
		if (combinedOutput.length && (combinedOutput.includes("Error\n") || combinedOutput.includes("Error:"))) {
			printBasic(combinedOutput, '\n');
			printGreen("[OK]", "PARSER : ");
			parserCount.passed++;
		} else {
			printBasic("\n");
			printRed("[FAILED]", "PARSER : ",
				" ⚠️ Your error message doesn't include Error ⚠️");
			printBasic("[n/a]","LEAKS  :")
			parserCount.failed++;
			parserCount.maps.push(file);
		}
	}
}

const checkLeaks = async file => {
	if (!parserCount.maps.includes(file)) {
		try {
			await exec(`${VALGRIND} --log-file=logs/LOG_${file} ${EXEC + file}`);
		} catch (error) {
			// Valgrind command completed (may have failed, which is expected for invalid maps)
		}
		
		try {
			const { stdout } = await exec(`${GREP} logs/LOG_${file} ${GREP2}`);
			if (stdout) {
				printBasic(stdout);
				printRed("[FAILED]", "LEAKS  : ");
				leaksCount.failed++;
				leaksCount.maps.push(file);
			}
		} catch (err) {
			printGreen("[OK]", "LEAKS  : ");
			leaksCount.passed++;
			exec(`rm ./logs/LOG_${file}`);
		}
	}
}

module.exports = {
	makeHeader,
	checkParser,
	checkLeaks
}
