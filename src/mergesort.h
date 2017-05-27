/* Sliding Block Puzzle
 *
 * The base of this code was taken from:
 * http://www.geeksforgeeks.org/merge-sort-for-linked-list/
 *
 * It has been modified accordingly by myself to work
 * with the necessary structs and data fields for my
 * code for the Sliding Block Puzzle.
 *
 * Code by:
 * Tim Monfette
*/

// Merge two sorted lists together and sort them
OpenList* SortedMerge(OpenList *a, OpenList *b)
{
  OpenList *result = NULL;

  // Base cases
  if (a == NULL)
    return(b);
  else if (b==NULL)
    return(a);

  // Pick either a or b, and recur
  if (a->o->heuristic <= b->o->heuristic)
  {
    result = a;
    result->next = SortedMerge(a->next, b);
  }
  else
  {
    result = b;
    result->next = SortedMerge(a, b->next);
  }
  return(result);
}

// Split a list into two halves for sorting
void FrontBackSplit(OpenList *source, OpenList **frontRef, OpenList **backRef)
{
  OpenList *fast;
  OpenList *slow;
  if (source==NULL || source->next==NULL)
  {
    // length < 2 cases
    *frontRef = source;
    *backRef = NULL;
  }
  else
  {
    slow = source;
    fast = source->next;

    // Advance fast two nodes, and advance slow one node
    while (fast != NULL)
    {
      fast = fast->next;
      if (fast != NULL)
      {
        slow = slow->next;
        fast = fast->next;
      }
    }

    // slow is before the midpoint in the list, so split it in two at that point.
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}

// Sorts the list by moving pointers and not altering data
void MergeSort(OpenList **headRef)
{
  OpenList *head = *headRef;
  OpenList *a;
  OpenList *b;

  // Base case -- length 0 or 1
  if ((head == NULL) || (head->next == NULL))
  {
    return;
  }

  // Split head into 'a' and 'b' sublists
  FrontBackSplit(head, &a, &b); 

  // Recursively sort the sublists
  MergeSort(&a);
  MergeSort(&b);

  // Merge the two sorted lists together
  *headRef = SortedMerge(a, b);
}
