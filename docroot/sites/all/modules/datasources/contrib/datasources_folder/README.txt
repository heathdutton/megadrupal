This module is more a POC to show how import a folder tree of contents (text+photo).

So you can manage to import recursively a structured content and keep track of references between texts, photos and children contents.

-o- root/
 |
 |-o- something/
   |
   |-o- section1/
   | |
   | |-o- file1
   |   |--- article.txt
   |   |--- photo1.jpg
   |   |--- photo2.jpg
   |   |
   |   |-o- content1
   |     |--- interview.txt
   |     |--- photo1.jpg
   |
   |-o- content2
   | |--- article.txt
   | |--- photo1.jpg
   | |--- photo2.jpg
   |
   |-o- content3
     |--- article.txt
     |--- photo1.jpg
     |--- photo2.jpg (photo)
     |--- photo2.txt (caption)
     |--- photo3.jpg