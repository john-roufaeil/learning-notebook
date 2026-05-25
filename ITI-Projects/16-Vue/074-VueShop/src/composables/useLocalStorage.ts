import { ref, watch } from "vue";

export const useLocalStorage = (key: string, defaultValue: any) => {
  const stored = localStorage.getItem(key);
  const data = ref(stored ? JSON.parse(stored) : defaultValue);

  watch(data, (val) => {
    localStorage.setItem(key, JSON.stringify(val))
  }, { deep: true });

  return data;
}