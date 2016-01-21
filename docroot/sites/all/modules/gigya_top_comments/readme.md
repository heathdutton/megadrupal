# gigya_top_comments

## Dependencies
- Contributed module dependencies ()
- Custom module dependencies ()

## Version / Author
1.0(d6) /Hans Gutknecht, hans.gutknecht@mlssoccer.com
1.1(d7) /Louis Jimenez, louis.jimenez@mlssoccer.com

## Description
To start using the Gigya Top Comments module, add your Gigya API key and Secret key to to the admin screen. Each time cron is run, the module will retrieve the top comment streams from Gigya and display the matching content in the Gigya Top Comments Block. If the block is not displaying content or you see the error message "Content not found. Stream ID does not match any node ID's" then your database may be out of date. Make sure you are working with the latest production copy of your db so that the streams ID's can be correctly match with nodes.