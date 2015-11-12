23-05-2013, Copyright by Cipix Internet. E-mail: info@cipix.nl.

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Known issues
 * Installation
 * Details

INTRODUCTION
------------

Current Maintainer: Bas van Meurs <bas@cipix.nl> or <bvanmeurs1985@gmail.com.

Provides i18n field synchronisation for a node_reference field.

The referenced nodes are translated into the correct language. If no such
language exists, it is ignored. This is the same functionality as is supported
by the taxonomy reference translation in i18n_taxonomy.

KNOWN ISSUES
------------

The Simpletest will fail currently because of a bug in the 'parent' class in the
i18n.test file. Hopefully this will be fixed quickly.

INSTALLATION
------------

Enable the module. Enable the node_reference fields that you want to keep
synchronized in the 'Synchonize translations' on the node type edit page.

Nothing will directly be done to the existing field reference, but the next time
you will save a node the node reference field(s) will automatically be
synchronized.

DETAILS
------------

Consider saving multilingual node A (in language 'en', notation A[en]) with a
node reference field in which nodes X[en], Y[en], Z[und] are referenced.
Consider A has language 'en', but also has a version with language 'nl' (A[nl]).
Consider X, Y are both 'en', while Z is 'und' (not translatable).
X has also been translated in 'nl'.

Now, when saving A[en], the node references in A[nl] are also updated.
The values of the reference field in A[nl] will be set to: X[nl] and Z[und].
There is no 'nl' version for Y[en] so it is not referenced.

Next, when opening A[nl] and saving it, it is undesirable that the reference
from A[en] to Y[en] is removed (because it's obscure for the user that this
happens). That's why node references not available in the currently saved node's
language will be preserved when synchronizing node_reference fields.
