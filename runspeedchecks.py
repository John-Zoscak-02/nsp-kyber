import subprocess
 
 
# If your shell script has shebang, 
# you can omit shell=True argument.
speedcheckoutput = subprocess.run(["~/git/nsp-kyber/runspeedchecks.sh", "&>", "runspeedchecks.out"], shell=True, capture_output=True)

print(speedcheckoutput)
