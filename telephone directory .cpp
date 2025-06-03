
#include <iostream>
#include<string>
#include<fstream>
using namespace std;
struct GraphNode {
	string contact;
	string connectionType;
	GraphNode* next;

	GraphNode(const string& c, const string& type) : contact(c), connectionType(type), next(nullptr) {}
};
class phoneDirectory {

private:
	string name;
	string phoneNumber;
	string email;
	bool isFavorite;
	string password;
	GraphNode* connections;

	phoneDirectory* next;
	phoneDirectory* prev;
public:
	phoneDirectory(string name, string phoneNumber, string email, bool f = false) {
		this->name = name;
		this->email = email;
		this->phoneNumber = phoneNumber;
		this->isFavorite = false;
		this->next = NULL;
		this->prev = NULL;
		password = "1234";
		this->connections = nullptr;
	}
	void addContact(phoneDirectory*& head, string name, string phoneNumber, string email, const string& filename) {

		phoneDirectory* newContact = new phoneDirectory(name, phoneNumber, email);

		if (isDuplicateContact(head, name, phoneNumber, email)) {
			cout << "Error: A contact with the same name, phone number, or email already exists.\n";
			delete newContact;
			return;
		}
		if (!isValidContact(phoneNumber)) {
			cout << "Invalid phone number! It should contain only digits and be 11 characters long.\n";
			delete newContact;
			return;
		}
		if (!isValidEmail(email)) {
			cout << "Invalid email! Ensure it contains '@' and a valid domain.\n";
			delete newContact;
			return;
		}
		if (!head) {
			head = newContact;
			cout << "Contact added successfully.\n";
			saveToFile(newContact, filename);
			return;
		}
		phoneDirectory* temp = head;
		phoneDirectory* prev = nullptr;
		string lowerName = toLowerCase(name);

		while (temp && toLowerCase(temp->name) < lowerName) {
			prev = temp;
			temp = temp->next;
		}

		if (!prev) {
			newContact->next = temp;
			if (temp) temp->prev = newContact;
			head = newContact;
		}
		else {
			prev->next = newContact;
			newContact->prev = prev;
			newContact->next = temp;
			if (temp) temp->prev = newContact;
		}

		cout << "Contact added successfully.\n";
		saveToFile(newContact, filename);
	}
	phoneDirectory* loadContactsFromFile(const string& filename) {
		phoneDirectory* head = nullptr;
		phoneDirectory* tail = nullptr;

		ifstream file(filename);
		if (!file) {
			cout << "Error: Could not open the file.\n";
			return nullptr;
		}

		string line, name, phoneNumber, email, connection, connectionType;
		phoneDirectory* newContact = nullptr;

		while (getline(file, line)) {
			if (line.find("Name:") == 0) {
				if (newContact) {
					if (!head) {
						head = newContact;
						tail = newContact;
					}
					else {
						tail->next = newContact;
						newContact->prev = tail;
						tail = newContact;
					}
				}

				name = line.substr(6);
				newContact = new phoneDirectory(name, "", "");
			}
			else if (line.find("Phone Number:") == 0) {
				if (newContact) newContact->phoneNumber = line.substr(14);
			}
			else if (line.find("Email:") == 0) {
				if (newContact) newContact->email = line.substr(7);
			}
			else if (line.find("Favorite:") == 0) {
				if (newContact) newContact->isFavorite = (line.substr(10) == "Yes");
			}
			else if (line.find("Connection:") == 0) {
				connectionType = line.substr(12, line.find(" of ") - 12);
				connection = line.substr(line.find(" of ") + 4);
				if (newContact) {
					GraphNode* newConnection = new GraphNode(connection, connectionType);
					if (!newContact->connections) {
						newContact->connections = newConnection;
					}
					else {
						GraphNode* connTail = newContact->connections;
						while (connTail->next) {
							connTail = connTail->next;
						}
						connTail->next = newConnection;
					}
				}
			}
		}

		if (newContact) {
			if (!head) {
				head = newContact;
			}
			else {
				tail->next = newContact;
				newContact->prev = tail;
			}
		}

		file.close();
		return head;
	}

	void saveToFile(phoneDirectory* newContact, const string& filename) {
		ofstream file(filename, ios::app);
		if (!file) {
			cout << "Error: Unable to open file for writing.\n";
			return;
		}
		file << "Name: " << newContact->name << "\n";
		file << "Phone Number: " << newContact->phoneNumber << "\n";
		file << "Email: " << newContact->email << "\n";
		file << "Favorite: " << (newContact->isFavorite ? "Yes" : "No") << "\n";
		file << "----------------------------------------\n";

		file.close();
		cout << "Contact saved to file successfully.\n";
	}
	void displayContacts(const string& contactsFile) {
		bool displayed = false;
		string line, name, phoneNumber, email, favorite, connection;
		ifstream file(contactsFile);

		cout << "+--------------------+----------------------------+---------------------+-----------------------+--------------+\n";
		cout << "| Name               | Connection                 | Phone Number        | Email                 | Favorite     |\n";
		cout << "+--------------------+----------------------------+---------------------+-----------------------+--------------+\n";

		if (file) {
			while (getline(file, line)) {
				if (line.find("Name:") == 0) {
					if (!name.empty()) {
						cout << "|" << name;
						for (int i = name.length(); i < 20; ++i)
							cout << " ";
						cout << "|" << connection;
						for (int i = connection.length(); i < 28; ++i)
							cout << " ";
						cout << "| " << phoneNumber;
						for (int i = phoneNumber.length(); i < 20; ++i)
							cout << " ";
						cout << "| " << email;
						for (int i = email.length(); i < 22; ++i)
							cout << " ";
						cout << "| " << favorite;
						for (int i = favorite.length(); i < 9; ++i)
							cout << " ";
						cout << "\n";
						cout << "+--------------------+----------------------------+---------------------+--------------------------+-----------+\n";
					}

					name = line.substr(6);  					phoneNumber.clear();     					email.clear();           					favorite.clear();        					connection.clear();
				}
				else if (line.find("Phone Number:") == 0) {
					phoneNumber = line.substr(14);
				}
				else if (line.find("Email:") == 0) {
					email = line.substr(7);
				}
				else if (line.find("Favorite:") == 0) {
					favorite = line.substr(10);
				}
				else if (line.find("Connection:") == 0) {
					if (!connection.empty()) {
						connection += "; ";
					}
					connection += line.substr(12);
				}
			}

			if (!name.empty()) {
				cout << "|" << name;
				for (int i = name.length(); i < 20; ++i) cout << " ";
				cout << "|" << connection;
				for (int i = connection.length(); i < 28; ++i) cout << " ";
				cout << "| " << phoneNumber;
				for (int i = phoneNumber.length(); i < 20; ++i) cout << " ";
				cout << "| " << email;
				for (int i = email.length(); i < 22; ++i) cout << " ";
				cout << "| " << favorite;
				for (int i = favorite.length(); i < 9; ++i) cout << " ";
				cout << "\n";
				cout << "+--------------------+----------------------------+---------------------+--------------------------+-----------+\n";
				displayed = true;
			}

			if (!displayed) {
				cout << "| No contacts available                                                                                     \n";
				cout << "+-----------------------------------------------------------+---------------------------------------------------+\n";
			}
		}
		else {
			cout << "Error: Unable to open the contacts file.\n";
		}

		file.close();
	}

	bool authenticate() {
		cout << "\n==============================================\n";
		cout << "         Welcome to Your Phone Directory\n";
		cout << "==============================================\n";
		cout << "          Organize and manage your contacts\n";
		cout << "          with ease and efficiency!\n\n";
		int attempts = 3;
		while (attempts > 0) {
			cout << "Enter your password to access the directory: ";
			string inputPassword;
			getline(cin, inputPassword);

			if (inputPassword == password) {
				cout << "Access granted!\n";
				return true;
			}
			else {
				attempts--;
				cout << "Incorrect password. Attempts remaining: " << attempts << endl;
			}

		}
		cout << "Too many incorrect attempts. Access denied.\n";
		return false;
	}

	string toLowerCase(const string& str) {
		string lowerstr;
		for (char c : str) {
			lowerstr += tolower(c);
		}
		return lowerstr;
	}

	bool isValidContact(const string& phoneNumber) {
		if (phoneNumber.length() != 11) return false;
		for (char c : phoneNumber) {
			if (!isdigit(c)) {
				return false;
			}
		}
		return true;
	}
	bool isValidEmail(const string& email) {
		int atPos = email.find('@');
		int dotPos = email.find('.', atPos);

		if (atPos == 0 || dotPos <= atPos + 1 || dotPos == email.length() - 1)
			return false;

		return true;
	}

	bool isDuplicateContact(phoneDirectory* head, const string& name, const string& phoneNumber, const string& email) {
		phoneDirectory* temp = head;
		while (temp) {
			if (toLowerCase(temp->name) == toLowerCase(name) || temp->phoneNumber == phoneNumber || toLowerCase(temp->email) == toLowerCase(email)) {
				return true;
			}
			temp = temp->next;
		}
		return false;
	}

	void toggleFavorite(const string& name, const string& filename) {
		phoneDirectory* head = loadContactsFromFile(filename);
		if (!head) {
			cout << "No contacts to toggle.\n";
			return;
		}
		phoneDirectory* temp = head;
		while (temp) {
			temp = temp->next;
		}
		temp = head;
		bool contactFound = false;

		while (temp) {
			string loadedName = toLowerCase(temp->name);
			string searchName = toLowerCase(name);

			if (loadedName == searchName) {
				cout << " Match found. Current favorite status: "
					<< (temp->isFavorite ? "Yes" : "No") << "\n";
				temp->isFavorite = !temp->isFavorite;
				contactFound = true;
				cout << "Contact " << (temp->isFavorite ? "marked as favorite.\n" : "removed from favorites.\n");
				break;
			}
			temp = temp->next;
		}

		if (!contactFound) {
			cout << "Contact not found in the directory.\n";
			return;
		}


		ofstream outFile(filename, ios::trunc);
		if (!outFile) {
			cout << "Error opening the file.\n";
			return;
		}

		phoneDirectory* current = head;
		while (current) {
			outFile << "Name: " << current->name << "\n";

			GraphNode* conn = current->connections;
			while (conn) {
				outFile << "Connection: " << conn->connectionType << " of " << conn->contact << "\n";
				conn = conn->next;
			}

			outFile << "Phone Number: " << current->phoneNumber << "\n";
			outFile << "Email: " << current->email << "\n";
			outFile << "Favorite: " << (current->isFavorite ? "Yes" : "No") << "\n";
			outFile << "----------------------------------------------\n";
			current = current->next;
		}

		outFile.close();

		while (head) {
			phoneDirectory* toDelete = head;
			head = head->next;
			delete toDelete;
		}
	}

	void displayFavorites(const string& contactsFile) {
		bool displayed = false;
		string line, name, phoneNumber, email, favorite;
		ifstream file(contactsFile);

		if (file) {
			while (getline(file, line)) {

				if (line.find("Name:") == 0) {
					name = line.substr(6);
				}
				else if (line.find("Phone Number:") == 0) {
					phoneNumber = line.substr(14);
				}
				else if (line.find("Email:") == 0) {
					email = line.substr(7);
				}
				else if (line.find("Favorite:") == 0) {
					favorite = line.substr(10);

					if (favorite == "Yes") {
						if (!displayed) {
							cout << "\n+--------------------------------------------------------+\n";
							cout << "|                    Favorite Contacts                   |\n";
							cout << "+---------------+------------------+---------------------+\n";
							cout << "|   Name        |      Phone       |     Email           |\n";
							cout << "+---------------+------------------+---------------------+\n";
							displayed = true;
						}
						cout << "| " << name << "   |   " << phoneNumber << "    |   " << email << "   \n";
						cout << "+---------------+------------------+---------------------+\n";
					}
				}
			}
			if (!displayed) {
				cout << "+---------------+------------------+---------------------+\n";
				cout << "|        No Favorite Contacts Available                  |\n";
				cout << "+--------------------------------------------------------+\n";
			}
		}
		else {
			cout << "Error: Unable to open the file.\n";
		}

		file.close();
	}

	void linkedListToArray(phoneDirectory* head, phoneDirectory* arr[], int& size) {
		phoneDirectory* current = head;
		while (current) {
			arr[size] = current;
			size++;
			current = current->next;
		}
	}

	static bool caseInsensitiveCompare(const string& str1, const string& str2) {
		if (str1.size() != str2.size()) return false;

		for (size_t i = 0; i < str1.size(); ++i) {
			if (tolower(str1[i]) != tolower(str2[i])) {
				return false;
			}
		}
		return true;
	}
	bool searchContact(const string& filename, const string& contactName) {
		phoneDirectory* head = loadContactsFromFile(filename);
		if (!head) {
			return false;
		}
		phoneDirectory* arr[100];  int size = 0;
		linkedListToArray(head, arr, size);

		int start = 0, end = size - 1;
		while (start <= end) {
			int mid = start + (end - start) / 2;

			bool isEqual = caseInsensitiveCompare(arr[mid]->name, contactName);
			if (isEqual) {
				cout << "Contact found: \n";
				cout << "Name: " << arr[mid]->name << endl;
				cout << "Phone: " << arr[mid]->phoneNumber << endl;
				cout << "Email: " << arr[mid]->email << endl;
				return true;
			}
			else {
				if (toLowerCase(arr[mid]->name) < toLowerCase(contactName)) {
					start = mid + 1;
				}
				else {
					end = mid - 1;
				}
			}
		}
		cout << "Contact not found.\n";
		return false;
	}

	bool deleteContactFromFile(const string& filename, string contactName) {
		ifstream inputFile(filename);
		if (!inputFile) {
			cout << "Error: Could not open file.\n";
			return false;
		}
		string line, content = "";
		bool skipContact = false;
		bool contactFound = false;

		while (getline(inputFile, line)) {

			if (line.find("Name:") == 0) {
				string contactNameInFile = line.substr(6);
				if (caseInsensitiveCompare(contactNameInFile, contactName)) {
					skipContact = true;
					contactFound = true;
				}
				else {
					skipContact = false;
				}
			}
			if (!skipContact) {
				content += line + "\n";
			}
			else if (line.find("=") == 0) {

				skipContact = false;
			}
		}
		inputFile.close();

		if (!contactFound) {
			cout << "Contact not found in the file.\n";
			return false;
		}
		ofstream outFile(filename, ios::trunc);
		if (!outFile) {
			cout << "Error: Could not open file for writing.\n";
			return false;
		}
		outFile << content;
		outFile.close();

		cout << "Contact deleted from the file.\n";
		return true;
	}
	bool editContact(const string& filename, string contactName) {
		phoneDirectory* head = loadContactsFromFile(filename);
		if (!head) {
			cout << "Error: Could not load contacts from file.\n";
			return false;
		}

		phoneDirectory* current = head;
		bool contactFound = false;

		while (current) {
			if (caseInsensitiveCompare(current->name, contactName)) {
				contactFound = true;

				cout << "Contact found: " << current->name << endl;
				string newName, newPhoneNumber, newEmail;
				bool newFavorite;

				cout << "Enter new Name (Leave empty to keep the current): ";
				getline(cin, newName);
				if (!newName.empty()) {
					current->name = newName;
				}

				cout << "Enter new Phone Number (Leave empty to keep the current): ";
				getline(cin, newPhoneNumber);
				if (!newPhoneNumber.empty()) {
					current->phoneNumber = newPhoneNumber;
				}

				cout << "Enter new Email (Leave empty to keep the current): ";
				getline(cin, newEmail);
				if (!newEmail.empty()) {
					current->email = newEmail;
				}

				cout << "Do you want to mark this contact as favorite? (1 for Yes, 0 for No): ";
				cin >> newFavorite;
				cin.ignore();
				current->isFavorite = newFavorite;

				break;
			}
			current = current->next;
		}

		if (!contactFound) {
			cout << "Contact not found.\n";
			return false;
		}

		ofstream outFile(filename, ios::trunc);
		if (!outFile) {
			cout << "Error: Could not open file for writing.\n";
			return false;
		}

		current = head;
		while (current) {
			outFile << "Name: " << current->name << "\n";
			GraphNode* conn = current->connections;
			while (conn) {
				outFile << "Connection: " << conn->connectionType << " of " << conn->contact << "\n";
				conn = conn->next;
			}
			outFile << "Phone Number: " << current->phoneNumber << "\n";
			outFile << "Email: " << current->email << "\n";
			outFile << "Favorite: " << (current->isFavorite ? "Yes" : "No") << "\n";



			outFile << "--------------------------------------------\n";
			current = current->next;
		}

		outFile.close();
		cout << "Contact updated successfully.\n";
		return true;
	}


	void selectionSortAndUpdateFile(const string& filename) {
		phoneDirectory* head = loadContactsFromFile(filename);
		if (!head) {
			cout << "No contacts to sort.\n";
			return;
		}
		phoneDirectory* i = head;
		while (i != nullptr) {
			phoneDirectory* minNode = i;
			phoneDirectory* j = i->next;
			while (j != nullptr) {
				if (toLowerCase(j->name) < toLowerCase(minNode->name)) {
					minNode = j;
				}
				j = j->next;
			}
			if (minNode != i) {
				swap(i->name, minNode->name);
				swap(i->phoneNumber, minNode->phoneNumber);
				swap(i->email, minNode->email);
				swap(i->isFavorite, minNode->isFavorite);

				GraphNode* tempConn = i->connections;
				i->connections = minNode->connections;
				minNode->connections = tempConn;
			}
			i = i->next;
		}
		ofstream outFile(filename, ios::trunc);
		if (!outFile) {
			cout << "Error opening file for writing.\n";
			return;
		}
		phoneDirectory* temp = head;
		while (temp != nullptr) {
			outFile << "Name: " << temp->name << "\n";

			GraphNode* conn = temp->connections;
			while (conn != nullptr) {
				outFile << "Connection: " << conn->connectionType << " of " << conn->contact << "\n";
				conn = conn->next;
			}
			outFile << "Phone Number: " << temp->phoneNumber << "\n";
			outFile << "Email: " << temp->email << "\n";
			outFile << "Favorite: " << (temp->isFavorite ? "Yes" : "No") << "\n";

			outFile << "----------------------------------------\n";
			temp = temp->next;
		}
		outFile.close();
		cout << "Contacts have been sorted and saved back to the file.\n";
	}
	string storedPassword;
	void setPassword() {
		string newPassword;

		while (true) {
			cout << "Set a new password (4-8 characters): ";
			getline(cin, newPassword);

			if (newPassword.length() >= 4 && newPassword.length() <= 8) {
				storedPassword = newPassword;
				cout << "Password updated successfully!\n";
				break;
			}
			else {
				cout << "Invalid password length. Please try again.\n";
			}
		}
	}
	void displayMenu() {
		cout << "----------------------------------------------\n";

		cout << "| [1] Add a New Contact                      |\n";
		cout << "| [2] View All Contacts                      |\n";
		cout << "| [3] Manage Your Contacts                   |\n";
		cout << "| [4] Sort Contacts by Name                  |\n";
		cout << "| [5] Manage Favourites                      |\n";
		cout << "| [6] Set Password                           |\n";
		cout << "| [7] Manage Connections                     |\n";
		cout << "| [8] Exit the Phone Directory               |\n";

		cout << "----------------------------------------------\n";
		cout << "Please choose an option (1-7): ";
	}
};
string toLowerCase(const string& str) {
	string result;
	for (char c : str) {
		result += (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
	}
	return result;
}
void saveConnectionToFile(const string& contact1, const string& contact2, const string& connectionType, const string& filename) {
	ifstream inFile(filename);
	if (!inFile) {
		cout << "Error: Unable to open file for reading.\n";
		return;
	}

	string line;
	string updatedContent;
	bool contact1Found = false;
	bool contact2Found = false;

	while (getline(inFile, line)) {

		if (line.find("Name: ") != string::npos) {
			string contactName = line.substr(6); 			contactName = toLowerCase(contactName);
			if (contactName == toLowerCase(contact1) && !contact1Found) {
				contact1Found = true;
				updatedContent += line + "\n";
				updatedContent += "Connection: " + connectionType + " of " + contact2 + "\n";
				while (getline(inFile, line) && line != "----------------------------------------------") {
					updatedContent += line + "\n";
				}
				updatedContent += "----------------------------------------------\n";
			}
			else if (contactName == toLowerCase(contact2) && !contact2Found) {
				contact2Found = true;
				updatedContent += line + "\n";
				updatedContent += "Connection: " + connectionType + " of " + contact1 + "\n";
				while (getline(inFile, line) && line != "----------------------------------------------") {
					updatedContent += line + "\n";
				}
				updatedContent += "----------------------------------------------\n";
			}
			else {
				updatedContent += line + "\n";
			}
		}
		else {
			updatedContent += line + "\n";
		}
	}

	inFile.close();
	if (!contact1Found && !contact2Found) {
		cout << "Contacts not found.\n";
		return;
	}

	ofstream outFile(filename, ios::trunc);
	if (!outFile) {
		cout << "Error: Unable to open file for writing.\n";
		return;
	}

	outFile << updatedContent;
	outFile.close();
}




void addConnection(GraphNode*& graphHead, const string& contact1, const string& contact2, const string& connectionType, const string& filename) {
	GraphNode* newNode1 = new GraphNode(contact2, connectionType);
	newNode1->next = graphHead;
	graphHead = newNode1;

	GraphNode* newNode2 = new GraphNode(contact1, connectionType);
	newNode2->next = graphHead;
	graphHead = newNode2;

	saveConnectionToFile(contact1, contact2, connectionType, filename);

	cout << "Connection added successfully: " << contact1 << " <-> " << contact2 << " (" << connectionType << ")\n";
}
void displayConnections(const string& filename) {
	ifstream inFile(filename);
	if (!inFile) {
		cout << "Error: Unable to open file to display connections.\n";
		return;
	}

	string line;
	bool displayingConnection = false;
	string currentName;
	string currentConnectionType;

	cout << "+------------------------------------------------------------+" << endl;
	cout << "                --- All Connections ---\n";
	cout << "+-------------------+----------------------------------------+" << endl;
	cout << "|Name         " << "      |Connection Type" << endl;
	cout << "+-------------------+----------------------------------------+" << endl;

	while (getline(inFile, line)) {
		if (line.find("Name: ") != string::npos) {
			currentName = line.substr(6);
		}

		if (line.find("Connection: ") != string::npos) {
			currentConnectionType = line.substr(12);  						cout << "|" << currentName << "          |";
			cout << currentConnectionType << endl;
			cout << "+------------------+-----------------------------------------+" << endl;

		}

	}

	inFile.close();
}
int main() {
	phoneDirectory* head = nullptr;
	phoneDirectory directoryManager("", "", "");
	int choice = 0;
	string name, email, contact;
	const string filename = "C:/Users/Nexgen/source/repos/Project/Telephone Directory/contacts.txt";
	GraphNode* graphHead = nullptr;
	string contact1, contact2, connectionType;
	phoneDirectory* temp;

	if (!directoryManager.authenticate()) {
		return 0;
	}

	do {
		directoryManager.displayMenu();

		while (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid choice! Please enter a number between 1 and 7: ";
		}
		cin.ignore();

		switch (choice) {

		case 1:
			cout << "Enter Name: ";
			getline(cin, name);
			cout << "Enter Phone Number: ";
			getline(cin, contact);

			while (!directoryManager.isValidContact(contact)) {
				cout << "Invalid phone number! Please re-enter: ";
				getline(cin, contact);
			}
			cout << "Enter Email: ";
			getline(cin, email);
			while (!directoryManager.isValidEmail(email)) {
				cout << "Invalid email! Please re-enter: ";
				getline(cin, email);
			}
			directoryManager.addContact(head, name, contact, email, filename);
			break;

		case 2:
			directoryManager.displayContacts(filename);
			break;

		case 3:
			int subChoice;
			cout << "\n==============================================\n";
			cout << "    Manage Contacts:\n";
			cout << "==============================================\n";
			cout << "----------------------------------------------\n";
			cout << "| 1.  Search by Name or Phone Number         |\n";
			cout << "| 2.  Delete Contact                         |\n";
			cout << "| 3.  Update Contact                         |\n";
			cout << "----------------------------------------------\n";

			cout << "Enter your choice: ";
			while (!(cin >> subChoice)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid choice! Please enter a number between 1 and 3: ";
			}
			cin.ignore();

			switch (subChoice) {
			case 1:
				cout << "\nEnter name of Contact to search: ";
				getline(cin, name);
				directoryManager.searchContact(filename, name);
				break;
			case 2:
				cout << "\nEnter name of Contact to delete: ";
				getline(cin, name);
				directoryManager.deleteContactFromFile(filename, name);
				break;
			case 3:
				cout << "\nEnter the name of the contact to update: ";
				getline(cin, name);
				directoryManager.editContact(filename, name);
				break;
			default:
				cout << "Invalid choice, please try again.\n";
				break;
			}
			break;

		case 4:
			directoryManager.selectionSortAndUpdateFile(filename);
			break;

		case 5:
			int favChoice;
			cout << "\n==============================================\n";
			cout << " Manage Favorites:\n";
			cout << "==============================================\n";
			cout << "----------------------------------------------\n";
			cout << "| 1. View Favorites                          |\n";
			cout << "| 2. Mark or Remove Favorites                |\n";
			cout << "----------------------------------------------\n";

			cout << "Enter your choice: ";
			while (!(cin >> favChoice)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid choice! Please enter a number between 1 and 2: ";
			}
			cin.ignore();

			switch (favChoice) {
			case 1:
				temp = directoryManager.loadContactsFromFile(filename);
				directoryManager.displayFavorites(filename);
				break;
			case 2:
				cout << "Enter name to mark/unmark as favorite: ";
				getline(cin, name);
				directoryManager.toggleFavorite(name, filename);
				break;
			default:
				cout << "Invalid choice, please try again.\n";
				break;
			}
			break;

		case 6:
			directoryManager.setPassword();
			break;

		case 7: {
			int connectionChoice;
			cout << "\n==============================================\n";
			cout << "    Manage Connections:\n";
			cout << "==============================================\n";
			cout << "----------------------------------------------\n";
			cout << "| 1.  View All Connections                   |\n";
			cout << "| 2.  Add New Connection                     |\n";
			cout << "----------------------------------------------\n";
			cout << "Enter your choice: ";
			while (!(cin >> connectionChoice)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Invalid choice! Please enter a number between 1 and 2: ";
			}
			cin.ignore();

			switch (connectionChoice) {
			case 1:  								displayConnections(filename);  				break;

			case 2:  				cout << "Enter the first contact name: ";
				getline(cin, contact1);
				cout << "Enter the second contact name: ";
				getline(cin, contact2);
				cout << "Enter the connection type (e.g., Friend, Family, Colleague): ";
				getline(cin, connectionType);

				addConnection(graphHead, contact1, contact2, connectionType, filename);
				break;
			default:
				cout << "\nInvalid choice! Please enter a number between 1 and 2.\n";
				break;
			}
		}
			  break;

		case 8:
			cout << "\nExiting Phone Directory. Goodbye!\n";
			break;

		default:
			cout << "\nInvalid choice! Please enter a valid number between 1 and 8.\n";
			break;
		}

	} while (choice != 8);

	return 0;
}

