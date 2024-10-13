const fs = require('node:fs/promises');
const zlib = require('node:zlib')
const path = require('node:path');

const workingDir = process.cwd();
const file = process.argv[2];

async function checkArgs() {
	let argCheck = false;
	//Need to check:
	// if the 3rd argument is defined
	// if a file exists at the argument
	// if the beginning of the file is PK (zip)
	// if there is a directory matching the file name with .denigma, fail.
	if (file !== undefined) {
		console.log('opening file')
		try {
			const testFile = await fs.readFile(process.argv[2]);
			argCheck = true;
			return testFile;
			
		} catch (err) {
			console.log(`--DENIGMA--: encountered error: ${err}`);
			process.exit();
		}
	}



	if (argCheck === false) {
		console.log(`--DENIGMA-- Decrypting the Enigma file:
usage: node denigma.js [inputFile.musx]

Denigma will create a folder named [inputFile.denigma] including all of the unzipped,
decrypted, and gunzipped xml contents of the file.  This is meant to be a quick utility
to streamline opening lots of files for analysis.
`);
		process.exit();
		
	}


}


async function main() {
	const file = await checkArgs();
	zlib.unzip(file,(a) => {console.log(a);})
}

main();



