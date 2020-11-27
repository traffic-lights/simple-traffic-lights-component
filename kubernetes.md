## EFEF

agera
### Create namespace

`kubectl create -f namespace.json`

`kubectl config view`

https://kubernetes.io/docs/tasks/administer-cluster/namespaces-walkthrough/

```
kubectl config set-context traffic --namespace=traffic \
  --cluster=docker-desktop \
  --user=docker-desktop
```

### What is LoadBalancer DNS address?
https://github.com/kubernetes/dns/blob/master/docs/specification.md

`rabbitmq.traffic.svc.cluster.local`
`<service-name>.<namespace>.svc.cluster.local`