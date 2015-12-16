DESCRIPTION
------------

This module provides interface for cleaning inline styles from text fields.

KEY FEATURES
------------

 * mass cleaning inline styles from selected text fields in batch mode
 * cleaning summary from fields with type 'text_with_summary'
 * generating css file with css rules that contains cutted inline styles
 * smart css rules generating from cutted inline styles (it means that similar styles are combined into one rule)

USE CASES
------------

 * you just imported content to your site but you don't need inline styles from imported content (you need only markup)
 * you want cut inline styles only from several text fields types in nodes. So you shouldn't edit each node
 * some other reasons why you want to cut inline styles from text fields
