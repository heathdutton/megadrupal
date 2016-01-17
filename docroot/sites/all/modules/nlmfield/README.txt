

API INFORMATION
---------------

This module stores NLM XML information about contributors. It provides 10 colmumns to store this information. 
There is one "xml" column which can be considred the "master" column from which all other column data is derived.
All other columns are "auxiliary" in that they do not store any data-of-record, but merely reflect what is the xml
column and provide easy ways to query, sort, and index the data.

Data is moved from the XML column to the auxiliary columns by using hook_field_presave, which in turn calls nlmfield_compute_values.
nlmfield_compute_values takes the XML data and extracts out the revelant data to be included in the other columns.

Much like the PHP "date()" function, NLM-Field provides the ability to extract useful data from the XML using tokenized character "bits".
These "bits" are defined in nlmfield_bits and can be added-to or modified using hook_nlmfield_bits_alter.

In addition to these bits which can be used directly in the "custom" formatter, we also provide a series of "presets" which are pre-packaged
strings with placeholder bit characters. Presets can be added-to or modified using hook_nlmfield_presets_alter


Examples of different presets: 

APA Style				:Masri, H. A.
MLA Style				:Masri, Heather A.
The Chicago Manual of Style: Long	:Heather A Masri
The Chicago Manual of Style: Short	:Masri
BlueBook - Short			:Masri
Bluebook - Long				:Heather A. Masri
ALWD Citation Manual			:Heather A. Masri
ASA Style				:Masri, Heather A
Harvard referencing			:Masri, H. A.
Vancouver System			:Masri HA

