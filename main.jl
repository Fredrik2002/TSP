
using JuMP
using Cbc

function solve_delta_tsp(distances)
    n = size(distances, 1)
    model = Model(Cbc.Optimizer)
    @variable(model, x[1:n, 1:n], Bin)  # Variable binaire pour indiquer si le trajet passe par l'arc (i, j)
    @variable(model, u[1:n])  # Variables de position
    @objective(model, Min, sum(distances[i, j] * x[i, j] for i in 1:n, j in 1:n)) 
    # Contrainte: on ne bloque pas sur la même ville
    @constraint(model, [i in 1:n,], x[i, i] == 0)
    # Contrainte: chaque ville est visitée exactement une fois
    @constraint(model, [i in 1:n], sum(x[i, j] for j in 1:n) == 1)
    @constraint(model, [j in 1:n], sum(x[i, j] for i in 1:n) == 1)
    # Contrainte pour éviter les sous-tours
    @constraint(model, [i=1:n, j=2:n], u[i] - u[j] + n * x[i, j] <= n - 1)
    # Résolution
    optimize!(model)
    if termination_status(model) == MOI.OPTIMAL
        selected_arcs = value.(x)
    end
    return value.(sum(distances[i, j] * x[i, j] for i in 1:n, j in 1:n))
end

using DelimitedFiles

M = readdlm("/home/e20230002969/Documents/TER/main1/instances.txt")
m, N = size(M)
k = Integer(m/N)
fichier_entree = open("/home/e20230002969/Documents/TER/main1/datas_exacte.csv", "r")
lignes = readlines(fichier_entree)
close(fichier_entree)


for i in 1:k
    d=M[(i-1)*N+1:i*N,:]
    
    t1 = time()
    println(solve_delta_tsp(d))
    elapsed_time = time()-t1
    
    lignes[i+2]*=string(round(elapsed_time, digits=3))
end

fichier_sortie = open("/home/e20230002969/Documents/TER/main1/datas_exacte.csv", "w")
write(fichier_sortie, join(lignes, "\n"))
close(fichier_sortie)