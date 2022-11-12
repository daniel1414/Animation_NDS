# Delete all files from fbx/build
rm -rf fbx/build
mkdir fbx/build

# Create the art files in fbx/build
for file in fbx/*.fbx; do
    D:\\Private\\Nintendo\\devkitPro\\tools\\bin\\fbx2bin -filepath=$(realpath $file) -outdir=$(PWD)\\fbx\\build -o=$(basename $file).bin
done

#read -p "Hit enter to continue"