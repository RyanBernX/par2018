/*! \struct MyNode
    \brief A chain list Node struct.
*/

/*! \typedef MyNode Node
    \brief typedef of mynode to node
*/

/*! \var int val;
    \brief a int variable.
*/

/*! \fn int add(int *a, int i, int j)
    \brief a add func

	\latexonly
	return $\sum_{k=i}^j a_k$
	\endlatexonly
*/

/*! \def MAX
    \brief a macro.
*/

#DEFINE MAX 100

struct MyNode
{
	int val; /*!< value of the node */
	struct MyNode next; /*!< pointer pointing to the next node. */
};

typedef MyNode Node;

int val;

int add(int *a, int i, int j);
