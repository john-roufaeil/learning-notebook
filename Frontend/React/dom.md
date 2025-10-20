# DOM

When the type of the element is a function, React calls it, passing the props to get back the
underlying elements. It keeps on performing the same operation recursivelyE on the result
until it gets a tree of DOM nodes, which React can render on the screen. This process is
called **reconciliation**, and it is used by both React DOM and React Native to create the user
interfaces of their respective platforms.

```jsx
canShowSecretData() {
 const { dataIsReady, isAdmin, userHasPermissions } = this.props
 return dataIsReady && (isAdmin || userHasPermissions)
}
<div>
 {this.canShowSecretData() && <SecretData />}
</div>
```
