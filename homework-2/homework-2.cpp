#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */


/**
 * Custom String class to represent strings
 */
class String {
    public:
        /**
         * Constructor to create a new String object from a C-string
         * @param cstr a C-string
         */
        String(const char* cstr) {
            len = strlen(cstr);
            str = new char[len + 1];
            strcpy(str, cstr);
        }

        /**
         * Copy constructor for String class
         * @param other another String object to be copied
         */
        String(const String& other) {
            len = other.len;
            str = new char[len + 1];
            strcpy(str, other.str);
        }

        /**
         * Destructor for String class
         */
        ~String() {
            delete[] str;
        } 

        // Comparison operators for String objects.
        bool operator==(const String& other) const {
            return strcmp(str, other.str) == 0;
        }

        bool operator<(const String& other) const {
            return strcmp(str, other.str) < 0;
        }

        bool operator>(const String& other) const {
            return strcmp(str, other.str) > 0;
        }        

        /**
         * C-string getter
         * @return returns the underlying C-string
         */
        const char* c_str() const {
            return str;
        }

    private:
        char* str;
        size_t len;        
};

/**
 * Class to represent a mail
 */
class CMail {
    public:
        
        /**
         * Constructor to create a new CMail object
         * @param from the sender's email address
         * @param to the recipient's email address
         * @param body the content of the email
         */
        CMail(const char* from, const char* to, const char* body) : m_from(from), m_to(to), m_body(body) {}
        
        /**
         * Copy constructor for CMail class
         * @param other another CMail object to be copied
         */
        CMail(const CMail& other) : m_from(other.m_from), m_to(other.m_to), m_body(other.m_body) {}
        
        // Comparison operator for checking equality of two CMail objects.
        bool operator==(const CMail& other) const {
            return m_from == other.m_from && m_to == other.m_to && m_body == other.m_body;
        }

        // Overloaded << operator for outputting a CMail object to an ostream.
        friend ostream& operator<<(ostream& os, const CMail& mail) {
            os << "From: " << mail.m_from.c_str();
            os << ", To: " << mail.m_to.c_str();
            os << ", Body: " << mail.m_body.c_str();

            return os;
        }

        /**
         * Get sender's email address
         * @return returns the sender's email address as a C-string
         */
        const char* getFrom() const {
            return m_from.c_str();
        }
        
        /**
         * Get recipient's email address
         * @return returns the recipient's email address as a C-string
         */
        const char* getTo() const {
            return m_to.c_str();
        }

    private:
        String m_from;
        String m_to;
        String m_body;

    private:
};

/**
 * Class to represent a linked list node containing a CMail object
 */
class NodeLdList {
    public:
        CMail mail;
        NodeLdList* next;

        /**
         * Constructor to create a new NodeLdList object
         * @param mail a CMail object
         */
        NodeLdList(const CMail& mail) : mail(mail), next(nullptr) {}

        /**
         * Copy constructor for NodeLdList class
         * @param other another NodeLdList object to be copied
         */
        NodeLdList(const NodeLdList& other) : mail(other.mail), next(nullptr) {
            if (other.next) {
                next = new NodeLdList(*other.next);
            }
        }

        /**
         * Destructor for NodeLdList class
         */
        ~NodeLdList() {
            delete next;
        }
};


/**
 * Class to represent a linked list of CMail objects
 */
class LinkedList {
    public:
        /**
         * Default constructor initializes an empty LinkedList.
         */
        LinkedList() : m_head(nullptr), m_tail(nullptr) {}
        
        /**
         * Copy constructor for creating a new LinkedList from an existing one.
         * @param other The source LinkedList to be copied.
         */
        LinkedList(const LinkedList& other) : m_head(nullptr), m_tail(nullptr) {
            if (other.m_head) {
                m_head = new NodeLdList(*other.m_head);
                NodeLdList* current = m_head;
                while (current->next) {
                    current = current->next;
                }
                m_tail = current;
            }
        }        

        /**
         * Destructor to free memory allocated for the linked list.
         */
        ~LinkedList() {
            delete m_head;
        }

        /**
         * Method to add a CMail object to the LinkedList.
         * @param mail The CMail object to be added.
         */
        void addMail(const CMail& mail) {
            NodeLdList* newNode = new NodeLdList(mail);
            if (m_head == nullptr) {
                m_head = newNode;
                m_tail = newNode;
            } else {
                m_tail->next = newNode;
                m_tail = newNode;
            }
        }

        /**
         * Getter method for the head node of the LinkedList.
         * @return The head node of the LinkedList.
         */
        NodeLdList* getHead() const {
            return m_head;
        }

    private:
        NodeLdList* m_head; // Pointer to the head node of the LinkedList
        NodeLdList* m_tail; // Pointer to the tail node of the LinkedList
};

/**
 * Class to represent a node of a binary search tree containing a String key and a LinkedList of CMail objects
*/
struct NodeBST {
    String key;
    LinkedList mails;
    NodeBST* left;
    NodeBST* right;

    /**
     * Constructor to create a new NodeBST object
     * @param key a String object representing the key
     */
    NodeBST(const String& key) : key(key), left(nullptr), right(nullptr) {}

    /**
     * Copy constructor for NodeBST class
     * @param other another NodeBST object to be copied
     */
    NodeBST(const NodeBST& other) : key(other.key), mails(other.mails), left(nullptr), right(nullptr) {
        if (other.left) {
            left = new NodeBST(*other.left);
        }
        if (other.right) {
            right = new NodeBST(*other.right);
        }
    }
};

/**
 * Class to represent a binary search tree for storing and managing CMail objects.
 * It allows inserting CMail objects into the tree and finding them based on their keys.
 */
class BinarySearchTree {
    public:
         /**
         * Default constructor that initializes an empty BinarySearchTree.
         */
        
        BinarySearchTree() : root(nullptr) {}

        /**
         * Copy constructor for creating a new BinarySearchTree from an existing one.
         * @param other The existing BinarySearchTree to be copied.
         */    
        BinarySearchTree(const BinarySearchTree& other) : root(nullptr) {
            if (other.root) {
                root = new NodeBST(*other.root);
            }
        }

        /**
         * Overloaded assignment operator for copying a BinarySearchTree.
         * @param other The existing BinarySearchTree to be copied.
         * @return A reference to the copied BinarySearchTree.
         */
        BinarySearchTree& operator=(const BinarySearchTree& other) {
            if (this == &other) {
                return *this;
            }
            clear(root);
            root = nullptr;
            if (other.root) {
                root = new NodeBST(*other.root);
            }
            return *this;
        }

        /**
         * Destructor to free memory allocated for the binary search tree.
         */
        ~BinarySearchTree() {
            clear(root);
        }

        /**
         * Method to add a CMail object to the BinarySearchTree.
         * @param mail The CMail object to be added.
         * @param from A boolean flag indicating whether to index by sender (true) or recipient (false).
         */
        void addMail(const CMail& mail, bool from) {
            const char* key = from ? mail.getFrom() : mail.getTo();
            root = insert(root, key, mail);
        }

        /**
         * Method to find a LinkedList of CMail objects by key.
         * @param key The key to search for.
         * @return A pointer to the LinkedList of CMail objects with the specified key, or nullptr if not found.
         */
        LinkedList* findMails(const String& key) const{
            NodeBST* node = search(root, key);
            return node ? &node->mails : nullptr;
        }

    private:
        NodeBST* root;

        /**
         * Helper method to insert a new NodeBST into the tree, based on the key and CMail object.
         * @param node The current node in the tree.
         * @param key The key to be inserted.
         * @param mail The CMail object to be inserted.
         * @return The updated node after insertion.
         */
        NodeBST* insert(NodeBST* node, const String& key, const CMail& mail) {
            if (node == nullptr) {
                node = new NodeBST(key);
                node->mails.addMail(mail);
                return node;
            }

            if (key < node->key) {
                node->left = insert(node->left, key, mail);
            } else if (key > node->key) {
                node->right = insert(node->right, key, mail);
            } else {
                node->mails.addMail(mail);
            }

            return node;
        }

        /**
         * Helper method to search for a NodeBST with the specified key in the tree.
         * @param node The current node in the tree.
         * @param key The key to search for.
         * @return The found NodeBST with the specified key, or nullptr if not found.
         */
        NodeBST* search(NodeBST* node, const String& key)  const{
            if (node == nullptr || node->key == key) {
                return node;
            }
            if (key < node->key) {
                return search(node->left, key);
            }
            return search(node->right, key);
        }

        /**
         * Helper method to clear the memory allocated for the binary search tree.
         * Recursively deletes all nodes in the tree.
         * @param node The current node in the tree.
         */
        void clear(NodeBST* node) {
            if (node == nullptr) return;
            clear(node->left);
            clear(node->right);
            delete node;
        }
};

/**
 * Class to represent an iterator for CMail objects in a linked list
 */
class CMailIterator {
    public:
        /**
         * Default constructor initializing an empty CMailIterator
         */
        CMailIterator() : curr(nullptr), copiedList(nullptr) {}

        /**
         * Constructor taking a NodeLdList pointer as an argument
         * @param node a pointer to a NodeLdList object
         */
        CMailIterator(NodeLdList* node) : curr(nullptr), copiedList(nullptr) {
            copyList(node);
        }

        /**
         * Copy constructor for CMailIterator class
         * @param other another CMailIterator object to be copied
         */
        CMailIterator(const CMailIterator& other) : curr(nullptr), copiedList(nullptr) {
            copyList(other.curr);
        }

        /**
         * Destructor for CMailIterator class
         */
        ~CMailIterator() {
            clearCopiedList();
        }

        /**
         * Overloaded not operator to check if the iterator is pointing to nullptr
         * @return true if curr is nullptr, false otherwise
         */
        bool operator!() const {
            return curr == nullptr;
        }

        /**
         * Overloaded bool operator to check if the iterator is pointing to a valid NodeLdList
         * @return true if curr is not nullptr, false otherwise
         */
        operator bool() const {
            return curr != nullptr;
        }
        
         /**
         * Overloaded prefix increment operator to advance the iterator to the next NodeLdList
         * @return a reference to the updated iterator
         */
        CMailIterator& operator++() {
            if (curr != nullptr) {
                curr = curr->next;
            }
            return *this;
        }

        /**
         * Overloaded dereference operator to access the CMail object the iterator is pointing to
         * @return a constant reference to the CMail object
         */
        const CMail& operator*() const {
            return curr->mail;
        }

    private:
        NodeLdList* curr; // Pointer to the current NodeLdList object in the iterator
        NodeLdList* copiedList; // Pointer to the copied linked list


        /**
         * Helper method to deep-copy the input LinkedList
         * @param node a pointer to a NodeLdList object
         */
        void copyList(NodeLdList* node) {
            clearCopiedList();
            NodeLdList** last = &copiedList;
            while (node) {
                *last = new NodeLdList(node->mail);
                last = &((*last)->next);
                node = node->next;
            }
            curr = copiedList;
        }

        /**
         * Helper method to clear the memory allocated for the copied LinkedList
         */
        void clearCopiedList() {
            delete copiedList;
            copiedList = nullptr;
        }
};

/**
 * Class CMailServer representing a mail server that manages sent and received emails.
 */
class CMailServer {
    public:
        // Default constructor for CMailServer class.
        CMailServer() = default;
        // Copy constructor for CMailServer class.
        CMailServer(const CMailServer& src) : inboxTree(src.inboxTree), outboxTree(src.outboxTree) {}

        /**
         * Method to send a CMail object, adding it to the outbox and inbox trees.
         * @param m a reference to a CMail object.
         */
        void sendMail(const CMail& m) {
            outboxTree.addMail(m, true);
            inboxTree.addMail(m, false);
        }

        /**
         * Overloaded assignment operator for the CMailServer class.
         * @param src a reference to another CMailServer object.
         * @return a reference to the current CMailServer object.
         */
        CMailServer& operator=(const CMailServer& src) {
            if (this == &src) {
                return *this;
            }            
            inboxTree = BinarySearchTree(src.inboxTree);
            outboxTree = BinarySearchTree(src.outboxTree);
            return *this;
        }

        /**
         * Method to return a CMailIterator for the outbox of a specified email address.
         * @param email a pointer to a const char string representing the email address.
         * @return a CMailIterator object pointing to the outbox.
         */
        CMailIterator outbox(const char* email) const {
            String emailStr(email);
            LinkedList* mails = outboxTree.findMails(emailStr);            
            return mails ? CMailIterator(mails->getHead()) : CMailIterator(nullptr);
            
        }

        /**
         * Method to return a CMailIterator for the inbox of a specified email address.
         * @param email a pointer to a const char string representing the email address.
         * @return a CMailIterator object pointing to the inbox.
         */
        CMailIterator inbox(const char* email) const {
            String emailStr(email);
            LinkedList* mails = inboxTree.findMails(emailStr);
            return mails ? CMailIterator(mails->getHead()) : CMailIterator(nullptr);
        }

    private:
        
        BinarySearchTree inboxTree; // Private member representing the inbox tree. 
        BinarySearchTree outboxTree; // Private member representing the outbox tree.
};


#ifndef __PROGTEST__
bool                         matchOutput                   ( const CMail     & m,
                                                             const char      * str )
{
  ostringstream oss;
  oss << m;
  return oss . str () == str;
}

int main ( void )
{
  char from[100], to[100], body[1024];

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
  CMailServer s0;
  s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "thomas", sizeof ( to ) );
  strncpy ( body, "another important mail", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "alice", sizeof ( to ) );
  strncpy ( body, "deadline notice", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
  s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
  CMailIterator i0 = s0 . inbox ( "alice" );
  assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i0 );

  CMailIterator i1 = s0 . inbox ( "john" );
  assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
  assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
  assert ( ! ++i1 );

  CMailIterator i2 = s0 . outbox ( "john" );
  assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ! ++i2 );

  CMailIterator i3 = s0 . outbox ( "thomas" );
  assert ( ! i3 );

  CMailIterator i4 = s0 . outbox ( "steve" );
  assert ( ! i4 );

  CMailIterator i5 = s0 . outbox ( "thomas" );
  s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! i5 );

  CMailIterator i6 = s0 . outbox ( "thomas" );
  assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
  assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
  assert ( ! ++i6 );

  CMailIterator i7 = s0 . inbox ( "alice" );
  s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
  assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i7 );

  CMailIterator i8 = s0 . inbox ( "alice" );
  assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ! ++i8 );

  CMailServer s1 ( s0 );
  s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
  s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
  CMailIterator i9 = s0 . inbox ( "alice" );
  assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ! ++i9 );

  CMailIterator i10 = s1 . inbox ( "alice" );
  assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
  assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
  assert ( ! ++i10 );

  CMailServer s2;
  s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
  CMailIterator i11 = s2 . inbox ( "alice" );
  assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
  assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
  assert ( ! ++i11 );

  s2 = s0;
  s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
  s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
  CMailIterator i12 = s0 . inbox ( "alice" );
  assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
  assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
  assert ( ! ++i12 );

  CMailIterator i13 = s2 . inbox ( "alice" );
  assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
  assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
  assert ( ! ++i13 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

