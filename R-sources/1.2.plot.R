library(ggplot2)

# plot temp and tempObj
ggplot(data=data, aes(x=Timestamp)) + 
  geom_line(aes(y=Temp, color="air temperature")) + 
  geom_line(aes(y=TempObj, color="object temperature"))

# plot the histogram for Temp
ggplot(data=data, aes(Temp)) +
  geom_histogram() +
  facet_grid(Hour ~ .)

# plot acceleration
ggplot(data=data, aes(x=Timestamp)) + 
  geom_line(aes(y=X, color="x")) + 
  geom_line(aes(y=Y, color="y")) + 
  geom_line(aes(y=Z, color="z")) 

# plot the histogram for Temp
ggplot(data=data, aes(X^2+Y^2+Z^2)) +
  geom_histogram() +
  facet_grid(Hour ~ .)


# mak
ggplot(data=data, aes(Temp)) +
        geom_histogram() +
        facet_grid(Hour ~ Board)

ggplot(data=data, aes(TempObj)) +
  geom_histogram() +
  facet_grid(Hour ~ Board)

ggplot(data=data, aes(TempObj-Temp)) +
        geom_histogram() +
        facet_grid(. ~ Board)


ggplot(data=data, aes(RSSI)) +
  geom_histogram() +
  facet_grid(Hour ~ Board)

ggplot(data=data, aes(x=Timestamp, y=TempObj)) + 
  geom_line(aes(y=Temp, color="a")) + 
  geom_line(aes(y=TempObj, color="b")) + 
  geom_smooth(method="lm",col=1,size=2) +
  facet_grid( . ~ Board )

ggplot(data=data, aes(x=Timestamp, y=RSSI)) + 
  geom_line(aes(y=RSSI, color="a")) + 
  geom_smooth(method="lm",col=1,size=2) +
  facet_grid( . ~ Board )
