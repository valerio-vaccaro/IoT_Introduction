# load dataframes foreach board
data1 <- read.csv("http://10.0.1.64:8001/api/dataset", header = FALSE)
data2 <- read.csv("http://10.0.1.64:8002/api/dataset", header = FALSE)
data3 <- read.csv("http://10.0.1.64:8003/api/dataset", header = FALSE)
data4 <- read.csv("http://10.0.1.64:8004/api/dataset", header = FALSE)
data5 <- read.csv("http://10.0.1.64:8005/api/dataset", header = FALSE)
data6 <- read.csv("http://10.0.1.64:8006/api/dataset", header = FALSE)
# combine all rows in a single dataframe
data <- rbind(data1, data2, data3, data4, data5, data6)
# give a useful name to every column
names(data) <- c("Timestamp", "Board", "Temp", "TempObj", "X", "Y", "Z", "RSSI", "Button1", "Button2")
# convert timestamp from Epoch to a POSIXct object
data$Timestamp <- as.POSIXct(data$Timestamp/1000,origin = "1970-01-01",tz = "CET")
# add column with hour
data$Hour <-  as.POSIXlt(data$Timestamp)$hour 

summary(data)

