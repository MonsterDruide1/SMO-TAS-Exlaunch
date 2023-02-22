target="$1"

if [ -z "$target" ]; then
    echo "Usage: deploy.sh <target>"
    exit 1
fi

# if all or yuzu, deploy to yuzu
if [ "$target" = "all" ] || [ "$target" = "yuzu" ]; then
    echo "Deploying to yuzu"
    cp build/subsdk9 /mnt/c/Users/Adrian/AppData/Roaming/yuzu/load/0100000000010000/bonk-damage/exefs/subsdk9
    cp build/main.npdm /mnt/c/Users/Adrian/AppData/Roaming/yuzu/load/0100000000010000/bonk-damage/exefs/main.npdm
fi
