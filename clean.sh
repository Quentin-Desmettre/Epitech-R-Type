if [ -z "$1" ]
then
    find build ! -path "build/_deps/*" -delete
elif [ "$1" = "-f" ]
then
    rm -rf build r-type_server r-type_client
else
    echo "Bad argument"
fi
