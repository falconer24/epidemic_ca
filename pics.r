cell.display <- function(G,S)
{
 png(S,width=1024,height=1024,bg='black');		# Set output as png
 par(new=F)
 image(1:ncol(G), 1:nrow(G), t(G), col=heat.colors(100),axes=FALSE,xlab='',ylab='')
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

dat2png <- function (Ng,M,N) 
{
 for (k in 1:Ng)
 	{
	 print(k)
	 S <- sprintf("./Life%s.dat",pad.zero(k,Ng))
	 print(S)
	 G <- matrix(as.matrix(read.table(S)),M,N)
	 S <- sprintf("./Life%s.png",pad.zero(k,Ng))
	 cell.display(G,S)
	 print(S)
	}
}
