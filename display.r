cell.display <- function(G,S,z)
{
 #h<-heat.colors(100)
 h <- rgb(seq(0,1,0.1),0.99,0.99)
 png(S,width=1024,height=1024,bg='black');		# Set output as png
 par(new=F)
 image(1:ncol(G), 1:nrow(G), t(G),zlim=z, col=rev(h),axes=FALSE,xlab='',ylab='')
 dev.off()
}


pad.zero <- function(n,max)
{
 nmax <- floor(log10(max))
 nthis <- floor(log10(n))
 t <- nmax - nthis
 
 s<-''
 if (t!=0)
 	for (i in 1:t)
 		s<- paste0('0',s)
 paste0(s,n)
}

minmax <-function(Ng,M,N)
{
 S <- sprintf("/tmp/Infected000.dat")
 G <- matrix(as.matrix(read.table(S)),M,N)
 sm <- 100
 sM <- 0
 for (k in 1:Ng)
 	{
	 #print(sm)
	 #print(sM)
	 S <- sprintf("/tmp/Infected%s.dat",pad.zero(k,Ng))
	 #print(S)
	 G<-matrix(as.matrix(read.table(S)),M,N)
	 sm <- min(sm,min(G))
	 sM <- max(sM,max(G))
	}
 sm <- max(0,sm)
 sM <- min(sM,100)
 c(sm,sM)
}

inf_dat2png <- function (Ng,M,N) 
{
 S <- sprintf("./Infected000.dat")
 print(S)
 G <- matrix(as.matrix(read.table(S)),M,N)
 S <- sprintf("./Infected000.png")
 cell.display(G,S,c(m,M))
 print(S)
 for (k in 1:Ng)
 	{
	 print(k)
	 S <- sprintf("./Infected%s.dat",pad.zero(k,Ng))
	 print(S)
	 G <- matrix(as.matrix(read.table(S)),M,N)
	 S <- sprintf("./Infected%s.png",pad.zero(k,Ng))
	 cell.display(G,S,c(m,M))
	 print(S)
	}
}

pop_dat2png <- function (Ng,M,N) 
{
 S <- sprintf("./Population000.dat")
 print(S)
 G <- matrix(as.matrix(read.table(S)),M,N)
 S <- sprintf("./Population000.png")
 cell.display(G,S,c(100,100000))
 print(S)
 for (k in 1:Ng)
 	{
	 print(k)
	 S <- sprintf("./Population%s.dat",pad.zero(k,Ng))
	 print(S)
	 G <- matrix(as.matrix(read.table(S)),M,N)
	 S <- sprintf("./Population%s.png",pad.zero(k,Ng))
	 cell.display(G,S,c(100,100000))
	 print(S)
	}
}

combo.display <- function(S,G1,z1,G2,z2)
{
 hP <- rgb(seq(0,1,0.1),0.99,0.99)
 hI <-heat.colors(100)
 png(S,width=2048,height=1024,bg='black');		# Set output as png
 par(new=F,mfrow=c(1,2))
 image(1:ncol(G1), 1:nrow(G1), t(G1),zlim=z1, col=rev(hP),axes=FALSE,xlab='',ylab='')
 image(1:ncol(G2), 1:nrow(G2), t(G2),zlim=z2, col=rev(hI),axes=FALSE,xlab='',ylab='')
 dev.off()
}

combined <-function(Ng,M,N,z)
{
 S1 <- sprintf("./Population000.dat")
 S2 <- sprintf("./Infected000.dat")
 print(S1)
 print(S2)
 G1 <- matrix(as.matrix(read.table(S1)),M,N)
 G2 <- matrix(as.matrix(read.table(S2)),M,N)
 
 S <- sprintf("./Epid000.png")
 
 combo.display(S,G1,c(100,100000),G2,z)
 
 print(S1)
 print(S2)
 for (k in 1:Ng)
 	{
	 print(k)
	 S1 <- sprintf("./Population%s.dat",pad.zero(k,Ng))
	 S2 <- sprintf("./Infected%s.dat",pad.zero(k,Ng))
	 print(S1)
	 print(S2)
	 G1 <- matrix(as.matrix(read.table(S1)),M,N)
	 G2 <- matrix(as.matrix(read.table(S2)),M,N)
	 S <- sprintf("./Epid%s.png",pad.zero(k,Ng))
	 
	 combo.display(S,G1,c(100,100000),G2,z)
	 
	 print(S1)
	 print(S2)
	}
}
