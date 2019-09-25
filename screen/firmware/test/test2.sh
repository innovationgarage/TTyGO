# Reset screen
echo -n -e "\x1bc"

cat <<EOF
********************************
*                              *
*                              *
*                              *
*                              *
********************************
EOF

echo -n -e "\x1b[5A\x1b[2COn the first line"
echo -n -e "\x1b[1E\x1b[3GOn the second line"
echo -n -e "\x1b[4;3HOn the muahahaha"
echo -n -e "\x1b[1E\x1b[2COn the forth line"
echo -n -e "\x1b[5;4H"
echo -n -e "\x1b[1A\x1b[1DOn the third line"
echo -n -e "\x1b[7;1H"

echo -n -e "\x1b[2;2H"
while true; do
  read -s -N 1 c1
  if [ "$c1" == "$(echo -n -e "\x1b")" ]; then
    read -s -N 1 c2
    if [ "$c2" == "[" ]; then
      read -s -N 1 c3

      if [ "$c3" == "A" ]; then
        echo -n -e "\x1b[2G "
        echo -n -e "\x1b[A\x1b[2GX"
      else
        if [ "$c3" == "B" ]; then
          echo -n -e "\x1b[2G "
          echo -n -e "\x1b[B\x1b[2GX"
        fi
      fi
    fi
  else
    if [ "$c1" == "$(echo -n -e "\n")" ]; then
      echo -n ">>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    fi
  fi
done
         
