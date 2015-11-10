library(ggplot2)

#data1 <- read.csv("http://10.0.1.64:8001/api/dataset", header = FALSE)
#data2 <- read.csv("http://10.0.1.64:8002/api/dataset", header = FALSE)
#data3 <- read.csv("http://10.0.1.64:8003/api/dataset", header = FALSE)
#data4 <- read.csv("http://10.0.1.64:8004/api/dataset", header = FALSE)
#data5 <- read.csv("http://10.0.1.64:8005/api/dataset", header = FALSE)
#data6 <- read.csv("http://10.0.1.64:8006/api/dataset", header = FALSE)

data <- rbind(data1, data2, data3, data4, data5, data6)


names(data) <- c("Timestamp", "Board", "Temp", "TempObj", "X", "Y", "Z", "RSSI", "Button1", "Button2")
data$Timestamp <- as.POSIXct(data$Timestamp/1000,origin = "1970-01-01",tz = "CET")
data$Hour <-  as.POSIXlt(data$Timestamp)$hour 

summary(data)
ggplot(data=data, aes(x=Timestamp)) + 
  geom_line(aes(y=X, color="a")) + 
  geom_line(aes(y=Y, color="b")) + 
  geom_line(aes(y=Z, color="c")) +
  facet_grid( ~ Board )



ggplot(data=data, aes(TempObj-Temp)) +
  geom_histogram() +
  facet_grid(Hour ~ Board)

ggplot(data=data, aes(TempObj)) +
  geom_histogram() +
  facet_grid(Hour ~ Board)

ggplot(data=data, aes(Temp)) +
  geom_histogram() +
  facet_grid(Hour ~ Board)

ggplot(data=data, aes(RSSI)) +
  geom_histogram() +
  facet_grid(Hour ~ Board)


ggplot(data=data, aes(x=Timestamp, y=TempObj)) + 
  geom_line(aes(y=Temp, color="a")) + 
  geom_line(aes(y=TempObj, color="b")) + 
  geom_smooth(method="lm",col=1,size=2) +
  facet_grid( ~ Board )

ggplot(data=data, aes(x=Timestamp, y=RSSI)) + 
  geom_line(aes(y=RSSI, color="a")) + 
  geom_smooth(method="lm",col=1,size=2) +
  facet_grid( ~ Board )
