
#Theodoros Katsantas, 1097459
#Aggelikh Douvri, 1097441
#Agaph Augoustinou, 109


#!/bin/bash

business_file="Businesses.csv"

while true; do
    # Εμφάνιση μενού
    echo "Επιλογές:"
    echo "[1] Επιλογή αρχείου επιχειρήσεων"
    echo "[2] Προβολή στοιχείων επιχείρησης"
    echo "[3] Αλλαγή στοιχείου επιχείρησης"
    echo "[4] Προβολή αρχείου"
    echo "[5] Αποθήκευση αρχείου"
    echo "[6] Έξοδος"

    read choice

    case $choice in
        1)
            echo "Δώστε το path του αρχείου επιχειρήσεων (Enter για default: $business_file):"
            read input_file
            business_file=${input_file:-"Businesses.csv"}
            ;;
   	2)
	    echo "Δώστε τον κωδικό της επιχείρησης:"
	    read business_code

	    # Check if the entered business code exists
	    if awk -F ',' -v code="$business_code" 'BEGIN{found=0} $1 == code {found=1; exit} END{exit !found}' $business_file; then
	        awk -F ',' -v code="$business_code" 'BEGIN{print "Κωδικός, Όνομα, Οδός, Πόλη, Ταχ. κώδικας, Γεωγρ. μήκος, Γεωγρ. πλάτος"} $1 == code {print $0}' $business_file
	    else
	        echo "Λάθος κωδικός επιχείρησης. Δοκιμάστε ξανά."
	    fi
	    ;;
         3)
    		echo "Δώστε τον κωδικό της επιχείρησης για αλλαγή στοιχείου:"
    		read business_code
    		echo "Δώστε τον αριθμό του στοιχείου που θέλετε να αλλάξετε (1-7):"
    		echo "[1] Κωδικός, [2] Όνομα, [3] Οδός, [4] Πόλη, [5] Ταχ. κώδικας, [6] Γεωγρ. μήκος, [7] Γεωγρ. πλάτος"
    		read field_number
    		echo "Δώστε τη νέα τιμή:"
    		read new_value

   	 # Use awk to update the specific field (name) in the row with the given business code
    	awk -v code="$business_code" -v field="$field_number" -v value="$new_value" -F ',' '
        	BEGIN {OFS = FS}
        	$1 == code {$field = value}
        	{print}
    	' "$business_file" > temp_file && mv temp_file "$business_file"
    	;;

        4)
            more $business_file
            ;;
        5)
    		echo "Δώστε το path για αποθήκευση του αρχείου (Enter για default: Businesses.csv):"
    		read output_file
    		output_file=${output_file:-"Businesses.csv"}

   		 # Check if the output file is the same as the input file
   		 if [ "$business_file" == "$output_file" ]; then
        	echo "Το αρχείο εισόδου και εξόδου είναι το ίδιο. Επιλέξτε διαφορετικό αρχείο εξόδου."
    		else
     	   	cp "$business_file" "$output_file"
        	echo "Το αρχείο αποθηκεύτηκε ως: $output_file"
    		fi
    		;;

        6)
            echo "Τέλος προγράμματος."
            exit 0
            ;;
        *)
            echo "Μη έγκυρη επιλογή. Προσπαθήστε ξανά."
            ;;
    esac
done
