# there is correlation between temp and tempObj???

# Temp = intercept + beta1 * TempObj + Error
model <- lm(Temp ~ TempObj, data=data)
summary(model)

# predict some points
rawdata = data.frame(TempObj=c(10, 20, 30))
predict(model, newdata = rawdata)

# RSSI impact on Temp?
# Temp = intercept + beta1 * TempObj + beta2 * RSSI + Error
model <- lm(Temp ~ TempObj + RSSI, data=data)
summary(model)

model <- lm(Temp ~ RSSI, data=data)
summary(model)

# with cross correlation 
cor(data$Temp, data$TempObj)
cor(data$Temp, data$RSSI)
