library(ggplot2)

# plot temp and tempObj
ggplot(data=data, aes(x=Timestamp)) + 
  geom_line(aes(y=Temp, color="air temperature")) + 
  geom_line(aes(y=TempObj, color="object temperature"))

# plot the histogram for Temp
ggplot(data=data, aes(Temp)) +
        geom_histogram() +
        facet_grid(Hour ~ .)

# if you have more than one board
ggplot(data=data, aes(Temp)) +
        geom_histogram() +
        facet_grid(Hour ~ Board)

# plot acceleration
ggplot(data=data, aes(x=Timestamp)) + 
  geom_line(aes(y=X, color="x")) + 
  geom_line(aes(y=Y, color="y")) + 
  geom_line(aes(y=Z, color="z")) 

# plot RSSI
ggplot(data=data, aes(x=Timestamp)) + 
  geom_line(aes(y=RSSI, color="RSSI"))

# plot the histogram for RSSI
ggplot(data=data, aes(RSSI)) +
   geom_histogram() +
   facet_grid(. ~ Board)